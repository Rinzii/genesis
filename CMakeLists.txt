cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX "-d")


project(genesis VERSION "0.0.1")

file(ARCHIVE_EXTRACT INPUT "${CMAKE_CURRENT_SOURCE_DIR}/tools/dxc.zip" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/tools")

# include the configuration file
include(cmake/Config.cmake)

if (GENESIS_OS_LINUX)
  # Set execute permissions for the extracted executable. For some reason it doesn't have them by default.
  file(
          COPY ${CMAKE_CURRENT_SOURCE_DIR}/tools/dxc/linux/bin/dxc
          DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/tools/dxc/linux/bin/dxc
          FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
  )
endif()

set(is_root_project OFF)

if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  set(is_root_project ON)
endif()

option(GENESIS_BUILD_EDITOR "Build the genesis editor (else only library)" ON)
option(GENESIS_BUILD_SHADERS "Build the genesis shaders" ON)

if(GENESIS_BUILD_EDITOR)
    set(GENESIS_BUILD_SHADERS ON)
endif()


add_subdirectory(ext)
add_subdirectory(engine)

if(GENESIS_BUILD_EDITOR)
  add_subdirectory(editor)
endif()
