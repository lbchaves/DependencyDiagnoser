cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Hint: how to install Boost packagee on Linux, tested on Debian, "sudo apt install libboost-all-dev"
set(Boost_INCLUDE_DIR /usr/local/boost_1_76_0)

project(DependencyDiagnoser)

include_directories(include)
set(SOURCES src/main.cpp)

SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

# If threads are necessary here they go, otherwise the lines below can be deleted:
find_package (Threads)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})


