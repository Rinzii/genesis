project(genesis-editor)

add_executable(genesis-editor)
target_link_libraries(genesis-editor PRIVATE
        genesis::lib
        genesis::game
		genesis::ext
        )

# Setup our include structure
target_include_directories(genesis
        PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}/include"
        PRIVATE src
)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(genesis-editor PRIVATE
          -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
          )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
  target_compile_options(genesis-editor PRIVATE
          /W4 /WX
          )
endif()

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

# HEADERS AND SOURCES
include(editor_list.cmake)
target_sources(genesis PRIVATE ${editor_headers})
add_subdirectory(src)
