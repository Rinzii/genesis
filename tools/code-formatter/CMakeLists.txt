cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(code-formatter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX "-d")

add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_sources(${PROJECT_NAME} PRIVATE
  main.cpp
)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
  )
endif()
