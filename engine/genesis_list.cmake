


set(core_base_config_headers
        include/gen/core/base/config/architecture.hpp
        include/gen/core/base/config/compiler.hpp
        include/gen/core/base/config/compilerTraits.hpp
        include/gen/core/base/config/defines.hpp
        include/gen/core/base/config/platform.hpp
        include/gen/core/base/config/settings.hpp
        )

set(core_base_headers
        ${core_base_config_headers}
        include/gen/core/base/base.hpp
        )

set(core_headers
        ${core_base_headers}
        include/gen/core/monoInstance.hpp
)

set(rendering_utils_headers
        include/gen/rendering/utils/debugUtils.hpp
        include/gen/rendering/utils/exceptions.hpp
        include/gen/rendering/utils/vkHelpers.hpp
        include/gen/rendering/utils/vulkanStructs.hpp
)

set(rendering_vk_headers
        include/gen/rendering/vk/commands/commandBuffer.hpp
        include/gen/rendering/vk/commands/commandPool.hpp
        include/gen/rendering/vk/devices/logicalDevice.hpp
        include/gen/rendering/vk/fence.hpp
        include/gen/rendering/vk/image.hpp
        include/gen/rendering/vk/devices/instance.hpp
        include/gen/rendering/vk/semaphore.hpp
        include/gen/rendering/vk/swapchain.hpp
)

set(rendering_headers
        ${rendering_utils_headers}
        ${rendering_vk_headers}
        include/gen/rendering/renderWindow.hpp
        include/gen/rendering/renderer.hpp
		)

set(inputs_headers
        include/gen/inputs/controller.hpp
		)
		
set(io_headers
        include/gen/io/fileAsync.hpp
        include/gen/io/file.hpp
        include/gen/io/fileUtils.hpp
        include/gen/io/fileHelper.hpp
        )

set(system_win32_headers
        include/gen/system/win32/details/minWindows.hpp
        include/gen/system/win32/details/postWinapi.hpp
        include/gen/system/win32/details/preWinapi.hpp
        include/gen/system/win32/windows.hpp
        )

set(system_headers
        ${system_win32_headers}
        include/gen/system/types.hpp
        )


set(util_headers
        include/gen/util/fixed_string.hpp
        include/gen/util/version.hpp
        )

set(logger_headers
        include/gen/logger/context.hpp
        include/gen/logger/instance.hpp
        include/gen/logger/level.hpp
        include/gen/logger/log.hpp
        include/gen/logger/sink.hpp
)

set(windowing_headers
        include/gen/windowing/window.hpp
        include/gen/windowing/windowExceptions.hpp
        )

# core header include
set(genesis_headers
        ${core_headers}
		${rendering_headers}
		${inputs_headers}
        ${io_headers}
        ${system_headers}
        ${util_headers}
        ${logger_headers}
        ${windowing_headers}
        include/gen/application.hpp
        include/gen/engine.hpp
        include/gen/core.hpp
        include/gen/time.hpp
        )


##### General consensus on how we setup header_list.cmake #####
#
# Following these suggestions are not required but by following them it makes managing this file far easier.
#
# 1. The lowest level of a directory should always be above where is is gonna be set.
# 2. Follow the naming convention of set(<dir>_<subdir>_<subsubdir>_<subsubsubdir>_headers
# 3. sets should always be lower case
# 4. Ever new set MUST end with _headers
# 5. genesis is reserved for the root header and should NEVER be put in any other sets inside of this file
# 6. ${VARS} should always be included before any actual files.
# 7. It is not required but preferred if you order your sets in the order that they appear in the folder structure
#       ideally in alphabetical order
#
#
#       Example:
#                   set(math_simd_internal_headers
#                           // includes go here
#                           )
#
#                   set(math_simd_headers
#                           ${math_simd_internal_headers}
#                           // includes go here
#                           )
#
#                   set(math_headers
#                           ${math_simd_headers}
#                           // includes go here
#                           )
#
#                   set(genesis_headers
#                           ${math_headers}
#                           // other root includes go here
#                           )
##########
