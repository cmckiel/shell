cmake_minimum_required(VERSION 3.13)

project(shell-starter-cpp)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard
set(CMAKE_BUILD_TYPE DEBUG)

add_executable(shell ${SOURCE_FILES})

target_include_directories(shell PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(shell PRIVATE readline)
