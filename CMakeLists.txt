cmake_minimum_required(VERSION 3.20)
project(CS-6120)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
  -Wall -Wextra -Werror -Wconversion -Wpedantic -Wshadow -Wunused
  -Wold-style-cast -Wnull-dereference -fno-rtti -fno-exceptions
)

enable_testing()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OPT_SOURCES
  src/opt/make-cfg.cpp
  src/opt/dce-opt.cpp
)

set(UTILS_SOURCES
  src/utils/basic_block.cpp
  src/utils/cfg.cpp
  src/utils/dce.cpp
  src/utils/function.cpp
  src/utils/instruction_utils.cpp
  src/utils/parser.cpp
  src/utils/program.cpp
)

set(MAIN_SOURCE
  src/utils/cfg.cpp
  src/utils/dce.cpp
)

add_executable(make-cfg
  src/opt/make-cfg.cpp
  ${UTILS_SOURCES}
)

target_include_directories(make-cfg PRIVATE
  ${CMAKE_SOURCE_DIR}/src/dep
  ${CMAKE_SOURCE_DIR}/src/opt
  ${CMAKE_SOURCE_DIR}/src/utils
)

add_executable(dce-opt
  src/opt/dce-opt.cpp
  ${UTILS_SOURCES}
)

target_include_directories(dce-opt PRIVATE
  ${CMAKE_SOURCE_DIR}/src/dep
  ${CMAKE_SOURCE_DIR}/src/opt
  ${CMAKE_SOURCE_DIR}/src/utils
)

find_program(LIT NAMES lit)
if(NOT LIT)
  message(FATAL_ERROR "lit not found.")
endif()

add_custom_target(check
  COMMAND ${LIT} -s ${CMAKE_SOURCE_DIR}/test
  DEPENDS make-cfg dce-opt
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
