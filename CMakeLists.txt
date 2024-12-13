cmake_minimum_required(VERSION 3.20)
project(CS-6120)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
  -Wall -Wextra -Werror -Wconversion -Wpedantic -Wshadow -Wunused
  -Wold-style-cast -Wnull-dereference
)

enable_testing()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLES
  make-cfg
)

set(OPT_SOURCES
  src/opt/make-cfg.cpp
)

set(UTILS_SOURCES
  src/utils/basic_block.cpp
  src/utils/instruction_utils.cpp
  src/utils/parser.cpp
)

set(MAIN_SOURCE
  src/utils/cfg.cpp
)

foreach(EXECUTABLE ${EXECUTABLES})
  add_executable(${EXECUTABLE}
    ${MAIN_SOURCE}
    ${UTILS_SOURCES}
    ${OPT_SOURCES}
  )

  target_include_directories(${EXECUTABLE} PRIVATE
    ${CMAKE_SOURCE_DIR}/src/dep
    ${CMAKE_SOURCE_DIR}/src/opt
    ${CMAKE_SOURCE_DIR}/src/utils
  )
endforeach()

find_program(LIT NAMES lit)
if(NOT LIT)
  message(FATAL_ERROR "lit not found.")
endif()

add_custom_target(check
  COMMAND ${LIT} -s ${CMAKE_SOURCE_DIR}/test
  DEPENDS ${EXECUTABLES}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
