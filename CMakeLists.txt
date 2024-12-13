cmake_minimum_required(VERSION 3.20.0)

project(CS-6120 VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB LIB_SOURCES lib/*.cpp)

add_executable(cfg cfg/cfg.cpp ${LIB_SOURCES})
