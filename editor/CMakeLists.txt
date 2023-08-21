project(genesis-editor)

### Currently we require that the editor has all spv binary files in the same directory as the executable
# This may change at a later date. For now, we copy the files to the build directory.

# Set the source and destination directories
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/editor/bin")

# Get a list of all .spv files in the source directory
file(GLOB SPV_FILES "${SOURCE_DIR}/*.spv")

# This target does the following:
#   - Create the destination directory if it does not exist
#   - Remove all .spv files from the destination directory if they exist.
#   - Copy all .spv files from the source directory to the destination directory

add_custom_target(copy_spv_files ALL
        COMMENT "Copying .spv files"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/editor/Debug/bin"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/editor/Release/bin"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/editor/RelWithDebInfo/bin"
        COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/editor/Debug/bin/*.spv"
        COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/editor/Release/bin/*.spv"
        COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/editor/RelWithDebInfo/bin/*.spv"
        COMMAND ${CMAKE_COMMAND} -E copy ${SPV_FILES} "${CMAKE_BINARY_DIR}/editor/Debug/bin"
        COMMAND ${CMAKE_COMMAND} -E copy ${SPV_FILES} "${CMAKE_BINARY_DIR}/editor/Release/bin"
        COMMAND ${CMAKE_COMMAND} -E copy ${SPV_FILES} "${CMAKE_BINARY_DIR}/editor/RelWithDebInfo/bin"
        DEPENDS ${SPV_FILES}
)

add_executable(genesis-editor)
add_dependencies(genesis-editor copy_spv_files) # The engine requires the .spv files to be copied before it can be built
target_link_libraries(genesis-editor PRIVATE
        genesis::lib
		genesis::ext
        )
target_include_directories(genesis-editor PRIVATE src)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(genesis-editor PRIVATE
          -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
          )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
  target_compile_options(genesis-editor PRIVATE
          /W4 /WX
          )
endif()


target_sources(genesis-editor PRIVATE
        src/main.cpp
        )

if(GENESIS_PCH)
  target_precompile_headers(genesis-editor PRIVATE
          <algorithm>
          <atomic>
          <chrono>
          <filesystem>
          <functional>
          <memory>
          <mutex>
          <span>
          <string>
          <unordered_map>
          <unordered_set>
          <variant>
          <vector>
          )
endif()
