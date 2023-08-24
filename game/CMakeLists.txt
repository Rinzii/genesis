project(genesis-game)

add_library(genesis-game)
add_library(genesis::game ALIAS genesis-game)

target_link_libraries(genesis-game PRIVATE
        genesis::lib
		genesis::ext
        )
target_include_directories(genesis-game
        PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}/include"
        PRIVATE src
)

if(CMAKE_GENERATOR MATCHES "^(Visual Studio)")
    target_compile_options(genesis-game INTERFACE /MP)
endif()


if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(genesis-game PRIVATE
          -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
          )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
  target_compile_options(genesis-game PRIVATE
          /W4 /WX
          )
endif()


if(GENESIS_PCH)
  target_precompile_headers(genesis-game PRIVATE
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
include(game_list.cmake)
target_sources(genesis PRIVATE ${game_headers})
add_subdirectory(src)