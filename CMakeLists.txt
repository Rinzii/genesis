cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX "-d")

set(genesis_root_dir ${CMAKE_CURRENT_SOURCE_DIR})


project(genesis VERSION "0.0.1")

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools/dxc")
  message(STATUS "DXC already extracted! Skipping...")
else()
  message(STATUS "Extracting DXC!")
  file(ARCHIVE_EXTRACT INPUT "${CMAKE_CURRENT_SOURCE_DIR}/tools/dxc.zip" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/tools")
endif()

# include the configuration file
include(cmake/Config.cmake)

if (GENESIS_OS_LINUX)
  # Set execute permissions for the extracted executable. For some reason it doesn't have them by default.
#  file(
#          COPY ${CMAKE_CURRENT_SOURCE_DIR}/tools/dxc/linux/bin/dxc
#          DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/tools/dxc/linux/bin
#          FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
#  )

  execute_process(
          COMMAND chmod +x ${CMAKE_CURRENT_SOURCE_DIR}/tools/dxc/linux/bin/dxc
  )
endif()

set(is_root_project OFF)

if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  set(is_root_project ON)
endif()

option(GENESIS_ENABLE_EDITOR "Set the runtime to instead build the editor instead of the default" ON)
option(GENESIS_ENABLE_VERBOSE_LOGGING "Enable verbose logging" OFF)
option(GENESIS_BUILD_RUNTIME "Build the genesis runtime (else only library)" ON)
option(GENESIS_BUILD_GAME "Build the genesis game" ON)
option(GENESIS_BUILD_SHADERS "Build the genesis shaders" ON)
option(GENESIS_BUILD_TOOLS "Build Genesis tools" ON)
option(GENESIS_AUTOFORMAT "Run code-formatter before building Genesis" ON)

if(NOT GENESIS_BUILD_TOOLS AND GENESIS_AUTOFORMAT)
  message(FATAL_ERROR "Cannot enable GENESIS_AUTOFORMAT without GENESIS_BUILD_TOOLS")
endif()

if ((GENESIS_ENABLE_EDITOR OR GENESIS_BUILD_RUNTIME) AND NOT GENESIS_BUILD_GAME)
    message(FATAL_ERROR "Cannot build runtime or editor without game")
endif()

if((GENESIS_ENABLE_EDITOR OR GENESIS_BUILD_RUNTIME) AND NOT GENESIS_BUILD_SHADERS)
  set(GENESIS_BUILD_SHADERS ON CACHE BOOL "Runtime requires the shaders" FORCE) # Editor requires the shaders
endif()



add_subdirectory(ext)
add_subdirectory(engine)

if (GENESIS_BUILD_GAME)
  add_subdirectory(game)
endif ()

# Currently we are only building the editor as our runtime, but eventually
# we will have a separate runtime and editor with the editor runtime essentially hooking
# a purpose built editor into the runtime.
if (GENESIS_BUILD_RUNTIME)
  if (GENESIS_ENABLE_EDITOR)
    add_subdirectory(editor)
  else ()
    add_subdirectory(editor) # TODO: Replace with a default runtime.
  endif ()
endif ()

if (GENESIS_BUILD_TOOLS)
  add_subdirectory(tools/code-formatter)

  if (GENESIS_AUTOFORMAT)
    add_custom_target(autoformat ALL
      code-formatter -q engine
      COMMAND code-formatter -q game/src
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      USES_TERMINAL
      COMMENT "Formatting sources"
    )

    add_dependencies(genesis autoformat)
    add_dependencies(genesis-game autoformat)
  endif()
endif()
