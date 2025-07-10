cmake_minimum_required(VERSION 3.20)
project(CS-6120)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
  -Wall -Wextra -Werror -Wconversion -Wpedantic -Wunused
  -Wold-style-cast -Wnull-dereference -fno-exceptions -g
)

enable_testing()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Sanitizers (address and undefined behavior)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=undefined")

set(OPT_SOURCES
  src/opt/make-cfg.cpp
  src/opt/dce-opt.cpp
  src/opt/lvn-opt.cpp
)

set(UTILS_SOURCES
  src/utils/basic_block.cpp
  src/utils/cfg.cpp
  src/utils/dce.cpp
  src/utils/lvn.cpp
  src/utils/function.cpp
  src/utils/instruction.cpp
  src/utils/parser.cpp
  src/utils/program.cpp
  src/utils/type.cpp
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

add_executable(lvn-opt
  src/opt/lvn-opt.cpp
  ${UTILS_SOURCES}
)
target_include_directories(lvn-opt PRIVATE
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
  DEPENDS make-cfg dce-opt lvn-opt
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
