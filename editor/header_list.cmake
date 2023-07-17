


set(editor_headers
        include/editor.hpp
		include/tab.hpp
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
