project(genesis-ext)

find_package(Threads)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
    # delete the src directory if it exists
    message(STATUS "Deleting ext/src directory!")
    file(REMOVE_RECURSE "${CMAKE_CURRENT_SOURCE_DIR}/src")
    message(STATUS "Extracting ext/src directory!")
    file(ARCHIVE_EXTRACT INPUT "${CMAKE_CURRENT_SOURCE_DIR}/src.zip" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")
else ()
    message(STATUS "Extracting ext/src directory!")
    file(ARCHIVE_EXTRACT INPUT "${CMAKE_CURRENT_SOURCE_DIR}/src.zip" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()

add_subdirectory(src/vk_dynamic)
add_library(vk-dynamic::vk-dynamic ALIAS vk-dynamic)

add_subdirectory(src/vma)
add_library(vma::vma ALIAS vma)

include(FetchContent)
FetchContent_Declare(
        mim
        GIT_REPOSITORY https://github.com/Rinzii/mim.git
        GIT_TAG       origin/next
)
FetchContent_MakeAvailable(mim)

add_subdirectory(src/dxc)
add_library(dxc::dxc ALIAS dxc)

set(GLFW_VULKAN_STATIC OFF)
set(GLFW_INSTALL OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
add_subdirectory(src/glfw)
add_library(glfw::glfw ALIAS glfw)

set(INSTALL_GTEST OFF) # May want to turn this on. Not 100% sure.
set(BUILD_GMOCK ON)
add_subdirectory(src/googletest)
add_library(gtest::gtest ALIAS gtest)
add_library(gmock::gmock ALIAS gmock)

add_subdirectory(src/imgui)
add_library(imgui::imgui ALIAS imgui)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(SPIRV_CROSS_CLI OFF)
set(SPIRV_CROSS_ENABLE_CPP OFF)
set(SPIRV_CROSS_ENABLE_C_API OFF)
set(SPIRV_CROSS_ENABLE_GLSL ON)
set(SPIRV_CROSS_ENABLE_HLSL ON)
set(SPIRV_CROSS_ENABLE_MSL OFF)
set(SPIRV_CROSS_ENABLE_REFLECT OFF)
set(SPIRV_CROSS_ENABLE_TESTS OFF)
set(SPIRV_CROSS_ENABLE_UTIL OFF)
set(SPIRV_CROSS_SHARED OFF)
set(SPIRV_CROSS_STATIC ON)
set(SPIRV_CROSS_SKIP_INSTALL ON)
add_subdirectory(src/spirv_cross)

add_subdirectory(src/stb)
add_library(stb::image ALIAS stb-image)

add_library(genesis-ext INTERFACE)
add_library(genesis::ext ALIAS genesis-ext)

target_link_libraries(genesis-ext INTERFACE
        vk-dynamic::vk-dynamic
        vma::vma
        glfw::glfw
        mim::mim
        gtest::gtest
        gmock::gmock
        imgui::imgui
        spirv-cross-hlsl
        stb::image

        dxc::dxc

        Threads::Threads
        )

