#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fcntl.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>

#include "CommandParser.hpp"
#include "Commands.hpp"

namespace fs = std::filesystem;

CommandParser::CommandParser() {
  shell_builtins_.emplace("exit", std::make_unique<ExitCommand>());
  shell_builtins_.emplace("echo", std::make_unique<EchoCommand>());
  shell_builtins_.emplace("type", std::make_unique<TypeCommand>(*this));
  shell_builtins_.emplace("pwd", std::make_unique<PwdCommand>());
  shell_builtins_.emplace("cd", std::make_unique<CdCommand>());
}

bool CommandParser::execute(const std::vector<std::string>& argv) {
  bool res = true;

  bool redirect = false;
  std::string redirect_file_name;

  if (argv.size() == 0) {
    return false;
  }

  std::string command = argv[0];
  std::vector<std::string> args(argv.begin() + 1, argv.end());

  for (auto it = args.begin(); it != args.end(); ++it) {
    if (*it == ">" || *it == "1>") {
      redirect = true;
      it++;
      redirect_file_name = *it;
      args.pop_back();
      args.pop_back();
      break;
    }
  }

  if (shell_builtins_.contains(command)) {
    auto& command_handler = shell_builtins_.at(command);
    command_handler->execute(args);
  }
  else if (std::string command_path; find_on_system(command, command_path)) {
    // Construct a C-style argument vector in prep for execv() call
    std::vector<char*> argv;

    // first arg of vector is always the program to execute.
    // push a non-const c-string version of `command_path`
    argv.push_back(const_cast<char*>(command_path.c_str()));

    for (const auto& arg : args) {
      // push a non-const c-string version into argv
      argv.push_back(const_cast<char*>(arg.c_str()));
    }
    // The arg vector must be terminated with nullptr as per the execv api
    argv.push_back(nullptr);

    // Fork the process
    pid_t pid = fork();

    if (pid == -1) {
      // failed to fork
      std::cerr << "Failed to fork child process" << std::endl;
      exit(EXIT_FAILURE);
    }
    else if (pid > 0) {
      // parent
      // pid is not zero, therefore we are in the parent process.
      int status;
      waitpid(pid, &status, 0);
    }
    else {
      // child
      // pid is zero, we are in child process, go ahead and exec the program.

      // Handle output redirection in child process.
      if (redirect) {
        int fd = open(redirect_file_name.c_str(), O_WRONLY | O_CREAT, 0666);
        if (fd == -1) {
          std::cerr << "Failed to open file named: " << redirect_file_name << std::endl;
          exit(EXIT_FAILURE);
        }
        // Assume we have an open file now.

        int dup_fd = dup2(fd, STDOUT_FILENO);
        if (dup_fd == -1) {
          std::cerr << "Failed to redirect stdout" << std::endl;
          exit(EXIT_FAILURE);
        }

        close(fd);
      }

      execv(argv[0], argv.data());
      exit(EXIT_FAILURE); // exec does not return
    }
  }
  else {
    std::cout << command << ": command not found" << std::endl;
    res = false;
  }

  return res;
}

bool CommandParser::find_builtin(const std::string& command) {
  return shell_builtins_.contains(command);
}

bool CommandParser::find_on_system(const std::string& command, std::string& command_path) {
  bool res = false;

  const char *path_env = std::getenv("PATH");

  if (path_env == nullptr) {
    std::cerr << "PATH environment variable not found." << std::endl;
    return false;
  }

  std::string path(path_env);

  std::vector<std::string> paths;
  std::string path_component;
  std::istringstream iss(path);

  while (std::getline(iss, path_component, ':')) {
    if (!path_component.empty()) {
      paths.push_back(path_component);
    }
  }

  for (const auto& directory : paths) {
    try {
      auto it = fs::directory_iterator(directory);

      auto found = std::find_if(it, fs::end(it), [&command](const auto& entry) {
          return entry.path().filename() == command;
      });

      if (found != fs::end(it)) {
        auto file = found->path();
        fs::file_status fstatus = fs::status(file);
        fs::perms fperms = fstatus.permissions();

        // Check if any executable bit is set (Owner, Group, or Others)
        bool is_executable = (fperms & fs::perms::owner_exec) != fs::perms::none ||
                             (fperms & fs::perms::group_exec) != fs::perms::none ||
                             (fperms & fs::perms::others_exec) != fs::perms::none;

        if (is_executable) {
          command_path = file;
          res = true;
          break;
        }
      }
    }
    catch (std::exception& e) {
      continue;
    }
  }

  return res;
}
