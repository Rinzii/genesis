include(CMakeDependentOption)

option(GENESIS_PCH "Use pre-compiled headers for the Genesis Engine" ON)
option(GENESIS_ENABLE_SIMD "Enable SIMD for Genesis" OFF)


add_library(genesis)
add_library(genesis::lib ALIAS genesis)

target_compile_definitions(genesis
        PUBLIC
        $<$<CONFIG:Debug>:GEN_DEBUG>
        $<$<CONFIG:RelWithDebInfo>:GEN_DEBUG>
        $<$<CONFIG:Release>:GEN_NDEBUG>
        )

if (GENESIS_ENABLE_SIMD)
  # This create an internal definition that allows our project to check if it can use simd.
  # If the code decides it can then GEN_SIMD will be defined along with a bunch of other SIMD related defines.
  # This definition is only used once to to allow GEN_SIMD to figure out if it can be enabled and nowhere else.
  add_compile_definitions(INTERNAL_GEN_SIMD_ENABLE)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
  # Make MSVC shut up about not actually deprecated functions
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif ()


# COMPILER FLAGS
if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(genesis PRIVATE
          -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
          )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
  target_compile_options(genesis PRIVATE
          /W4 /WX
          )
endif()

if (GENESIS_BUILD_SHADERS)

  # Set the shader model to use when compiling HLSL shaders - minimum is 6_0
  set(TARGET_SHADER_MODEL "6_0" CACHE STRING "The shader model to use when compiling HLSL shaders")

  if (NOT TARGET_SHADER_MODEL MATCHES "^[0-9]+_[0-9]+$")
    message(FATAL_ERROR "Invalid shader model: ${TARGET_SHADER_MODEL}")
  endif ()

  if (${TARGET_SHADER_MODEL} VERSION_LESS "6_0")
    message(FATAL_ERROR "Shader model 6_0 or higher is required for DXC")
  endif ()



  # Identify the DXC executable path based on the current platform
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

  # Set the input and output directories
  set(INPUT_DIRECTORY "${genesis_root_dir}/data/ shaders")
  set(OUTPUT_DIRECTORY "${genesis_root_dir}/bin/shaders")

  # Ensure the output directory exists
  file(MAKE_DIRECTORY ${OUTPUT_DIRECTORY})

  # Example command to compile HLSL shader to SPIR-V
  # dxc.exe -spirv -T vs_6_0 -E VSMain .\simple.hlsl -Fo .\simple.vert.spv

  # If building with Ninja then just build the CompileShaders target.
  # You can just run vs22 normally and it will run CompileShaders on build.

  # Even though we can technically have our spirv files be only a singular source.
  # It is much simpler implementation wise to just have two binary files from spirv.
  # Though we still compile both Fragment and Vertex Shaders in a singular HLSL file for convenience.
  # Inside of the HLSL file your Vertex Shader main MUST be called VSMain and your Pixel (Fragment) Shader main MUST be called PSMain
  # Currently we only use the 6.0 shader model of HLSL but this can go all the way to the most modern version.
  add_custom_target(CmakeCompileShaders
          COMMENT "Compile all HLSL shaders to SPIR-V"
          )

  # Find all shader files in the input directory
  file(GLOB_RECURSE SHADER_SOURCE_FILES
          LIST_DIRECTORIES false
          CONFIGURE_DEPENDS
          "${INPUT_DIRECTORY}/*.hlsl"
          )

  foreach(SHADER_SOURCE_FILE ${SHADER_SOURCE_FILES})
    get_filename_component(SHADER_NAME ${SHADER_SOURCE_FILE} NAME)
    get_filename_component(SHADER_EXTENSION ${SHADER_NAME} EXT)

    if (${SHADER_EXTENSION} STREQUAL ".hlsl")
      if (${SHADER_NAME} MATCHES ".*VS\\.hlsl$") # Vertex Shader
        set(SHADER_TYPE "vs") # Vertex Shader
      elseif (${SHADER_NAME} MATCHES ".*PS\\.hlsl$") # Pixel Shader
        set(SHADER_TYPE "ps") # Pixel (Fragment) Shader
      elseif (${SHADER_NAME} MATCHES ".*CS\\.hlsl$") # Compute Shader
        set(SHADER_TYPE "cs") # Compute Shader
      elseif (${SHADER_NAME} MATCHES ".*GS\\.hlsl$") # Geometry Shader
        set(SHADER_TYPE "gs") # Geometry Shader
      elseif (${SHADER_NAME} MATCHES ".*DS\\.hlsl$") # Domain Shader
        set(SHADER_TYPE "ds") # Domain Shader
      elseif (${SHADER_NAME} MATCHES ".*HS\\.hlsl$") # Hull Shader
        set(SHADER_TYPE "hs") # Hull Shader
      elseif (${SHADER_NAME} MATCHES ".*LIB\\.hlsl$") # Library Shader - may not work with spirv
        if (${TARGET_SHADER_MODEL} VERSION_LESS "6_3") # Library shaders require shader model 6_3 or higher
          message(FATAL_ERROR "Shader model 6_0 or higher is required for file: ${SHADER_SOURCE_FILE}")
        endif ()
        message(WARNING "WARNING: Library shaders may not work with spirv")
        set(SHADER_TYPE "lib")
      else()
        set(SHADER_TYPE "UNKNOWN")
      endif()

      set_source_files_properties(${SHADER_SOURCE_FILE} PROPERTIES ShaderType ${SHADER_TYPE})
    endif()
  endforeach()


  # Iterate over each source file and add a custom command for conversion
  foreach(SHADER_SOURCE_FILE ${SHADER_SOURCE_FILES})
    # Get the filename without extension
    get_filename_component(SHADER_NAME ${SHADER_SOURCE_FILE} NAME_WE)
    get_source_file_property(SHADER_TYPE ${SHADER_SOURCE_FILE} ShaderType)

    # Sometimes we may have shader files used by other shaders that we don't want to compile directly.
    if (${SHADER_TYPE} STREQUAL "UNKNOWN")
      message(STATUS "DXC: Unknown shader type for file: ${SHADER_SOURCE_FILE}")
      continue()
    endif ()

    message(STATUS "Compiling ${SHADER_NAME} as ${SHADER_TYPE}")

    # Convert SHADER_SOURCE_FILE to absolute path
    get_filename_component(ABSOLUTE_SHADER_SOURCE_FILE ${SHADER_SOURCE_FILE} ABSOLUTE)

    # Define the output file path
    set(OUTPUT_SHADER_FILE ${OUTPUT_DIRECTORY}/${SHADER_NAME})

    # Add a custom command for each shader file.
    add_custom_command(
            TARGET CmakeCompileShaders
            COMMAND ${DXC_EXECUTABLE_PATH} -spirv -T ${SHADER_TYPE}_${TARGET_SHADER_MODEL} -E main ${ABSOLUTE_SHADER_SOURCE_FILE} -Fo ${OUTPUT_SHADER_FILE}.spv
            DEPENDS ${SHADER_SOURCE_FILE}
            COMMENT "Compiling ${SHADER_SOURCE_FILE} to SPIR-V"
            VERBATIM
    )
  endforeach()

  add_dependencies(genesis CmakeCompileShaders)
endif ()


# COMPILE FEATURES
target_compile_features(genesis PUBLIC cxx_std_20)

# LINK LIBRARIES
target_link_libraries(genesis
        PUBLIC
        mim::mim
        imgui::imgui
        glfw::glfw
        vk-dynamic::vk-dynamic

        PRIVATE
        genesis::ext
        )

# Setup our include structure
target_include_directories(genesis
        PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}/include"
        PRIVATE src
        )

# Generate build version header
configure_file(src/genesis_version.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/include/gen/genesis_version.hpp" @ONLY)

if(CMAKE_GENERATOR MATCHES "^(Visual Studio)")
    target_compile_options(genesis INTERFACE /MP)
endif()


# PRECOMPILED HEADERS
if(GENESIS_PCH)
    target_precompile_headers(genesis PRIVATE
            <vulkan/vulkan.hpp>
            <vk_mem_alloc.h>
            <algorithm>
            <atomic>
            <chrono>
            <filesystem>
            <functional>
            <future>
            <memory>
            <mutex>
            <optional>
            <span>
            <string>
            <unordered_map>
            <unordered_set>
            <variant>
            <vector>
            )
endif()
message(STATUS "PRECOMPILED HEADERS: ${GENESIS_PCH}")


# HEADERS AND SOURCES
include(genesis_list.cmake)
target_sources(genesis PRIVATE ${genesis_headers})
add_subdirectory(src)

