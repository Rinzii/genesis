// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// clang-format off

#ifndef GEN_TRUE
	#define GEN_TRUE 1
#endif

#ifndef GEN_FALSE
	#define GEN_FALSE 0
#endif


#ifndef GEN_IMPORT
	#if defined(GEN_COMPILER_MSVC)
		#define GEN_IMPORT __declspec(dllimport)
	#elif defined(GEN_COMPILER_GNUC) || defined(GEN_COMPILER_CLANG)
		#define GEN_IMPORT __attribute__((visibility("default")))
	#else
		#define GEN_IMPORT
	#endif
#endif


#ifndef GEN_EXPORT
	#if defined(_MSC_VER)
		#define GEN_EXPORT __declspec(dllexport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define GEN_EXPORT __attribute__((visibility("default")))
	#else
		#define GEN_EXPORT
	#endif
#endif


// For Visual C++ compilers, we also need to turn off this annoying C4251 warning
#ifdef _MSC_VER

	#pragma warning(disable : 4251)

#endif


// Define portable import / export macros
#if defined(GEN_DLL)
	#if defined(GEN_DLL_EXPORT)
		#define GEN_API GEN_EXPORT
	#else
		#define GEN_API GEN_IMPORT
	#endif
#else
	#define GEN_API
#endif


// Portable debug macro - this should be defined in cmake but just in case double check
#if !defined(NDEBUG)
	#if !defined(GEN_DEBUG)
		#define GEN_DEBUG
	#endif
#endif


namespace gen {
constexpr bool debug_v =
#if defined(GEN_DEBUG)
	true;
#else
	false;
#endif

constexpr bool freetype_v =
#if defined(GEN_USE_FREETYPE)
	true;
#else
	false;
#endif
} // namespace gen
