


set(base_config_headers
        include/base/config/architecture.hpp
        include/base/config/compiler.hpp
        include/base/config/compilerTraits.hpp
        include/base/config/defines.hpp
        include/base/config/platform.hpp
        include/base/config/settings.hpp
        )

set(base_headers
        ${base_config_headers}
        include/base/base.hpp
        )

set(graphics_headers
		include/graphics/pipeline.hpp
		)

set(io_headers
        include/io/fileAsync.hpp
        include/io/file.hpp
        include/io/fileHelper.hpp
        )

set(system_win32_headers
        include/system/win32/minWindows.hpp
        include/system/win32/postWinapi.hpp
        include/system/win32/preWinapi.hpp
        include/system/win32/windowsHeader.hpp
        )

set(system_headers
        ${system_win32_headers}
        include/system/types.hpp
        )


set(util_headers
        include/util/fixed_string.hpp
        include/util/version.hpp
        )

set(logger_headers
        include/logger/context.hpp
        include/logger/instance.hpp
        include/logger/level.hpp
        include/logger/log.hpp
        include/logger/sink.hpp
)

set(windowing_headers
        include/windowing/window.hpp
        )

# core header include
set(genesis_headers
        ${base_headers}
		#${graphics_headers}
        ${io_headers}
        ${system_headers}
        ${util_headers}
        ${logger_headers}
        ${windowing_headers}
        include/application.hpp
        include/core.hpp
        include/log.hpp
		include/time.hpp
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
