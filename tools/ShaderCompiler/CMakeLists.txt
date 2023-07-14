cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ShaderCompiler)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE shader_compiler.cpp)

if (GENESIS_OS_WINDOWS)
  if (GENESIS_ARCH_X64)
    set(DXC_EXECUTABLE_PATH "${CMAKE_SOURCE_DIR}/tools/dxc/windows/bin/x64/dxc.exe")
  elseif (GENESIS_ARCH_X86)
    set(DXC_EXECUTABLE_PATH "${CMAKE_SOURCE_DIR}/tools/dxc/windows/bin/x86/dxc.exe")
  elseif (GENESIS_ARCH_ARM OR GENESIS_ARCH_ARM64)
    set(DXC_EXECUTABLE_PATH "${CMAKE_SOURCE_DIR}/tools/dxc/windows/bin/arm64/dxc.exe")
  else()
    message(FATAL_ERROR "DXC does not support this platform")
  endif ()
elseif (GENESIS_OS_LINUX)
  set(DXC_EXECUTABLE_PATH "${CMAKE_SOURCE_DIR}/tools/dxc/linux/bin/dxc")
elseif (GENESIS_OS_MACOS)
  message(STATUS "Genesis has not tested DXC with MacOS and it may not work at all")
  set(DXC_EXECUTABLE_PATH "${CMAKE_SOURCE_DIR}/tools/dxc/linux/bin/dxc")
else()
  message(FATAL_ERROR "DXC does not support this platform")
endif ()

target_compile_definitions(${PROJECT_NAME} PRIVATE DXC_PATH="${DXC_EXECUTABLE_PATH}")

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(${PROJECT_NAME} PRIVATE
          -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
          )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            /W4 /WX
            )
endif()
