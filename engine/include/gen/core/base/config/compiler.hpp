// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

/*
 * Currently supported defines include:
 *      GEN_COMPILER_MSVC
 *      GEN_COMPILER_GCC
 *      GEN_COMPILER_CLANG
 *      GEN_COMPILER_CLANG_CL
 *
 *      GEN_COMPILER_VERSION = <integer>
 *      GEN_COMPILER_NAME = <string>
 *      GEN_COMPILER_STRING = <string>
 */

// Note: This is used to generate the GEN_COMPILER_STRING macros
#ifndef INTERNAL_STRINGIFY
	#define INTERNAL_STRINGIFY(x) INTERNAL_PRIMITIVE_STRINGIFY(x)
#endif
#ifndef INTERNAL_PRIMITIVE_STRINGIFY
	#define INTERNAL_PRIMITIVE_STRINGIFY(x) #x
#endif

/// GEN_COMPILER_CPP17_ENABLED
#if !defined(GEN_COMPILER_CPP17_ENABLED) && defined(__cplusplus)
	#if (__cplusplus >= 201703L)
		#define GEN_COMPILER_CPP17_ENABLED 1
	#elif defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L) // C++17+
		#define GEN_COMPILER_CPP17_ENABLED 1
	#endif
#endif

/// GEN_COMPILER_CPP20_ENABLED
// This check could have some issues with some compilers as a few have decided not to
// define __cplusplus correctly until they are fully supporting C++20, but this should
// not matter to use for our purposes.
#if !defined(GEN_COMPILER_CPP20_ENABLED) && defined(__cplusplus)
	#if (__cplusplus >= 202002L)
		#define GEN_COMPILER_CPP20_ENABLED 1
	#elif defined(_MSVC_LANG) && (_MSVC_LANG >= 202002L) // C++20+
		#define GEN_COMPILER_CPP20_ENABLED 1
	#endif
#endif

// Clang's GCC-compatible driver.
#if defined(__clang__) && !defined(_MSC_VER)
	#define GEN_COMPILER_CLANG	 1
	#define GEN_COMPILER_VERSION (__clang_major__ * 100 + __clang_minor__)
	#define GEN_COMPILER_NAME	 "clang"
	#define GEN_COMPILER_STRING	 GEN_COMPILER_NAME __clang_version__

// GCC (a.k.a. GNUC)
#elif defined(__GNUC__)
	// Define interchangeably between GCC and GNUC. Though GEN_COMPILER_GCC is preferred.
	#define GEN_COMPILER_GCC	 1
	#define GEN_COMPILER_GNUC	 1
	#define GEN_COMPILER_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
	#define GEN_COMPILER_NAME	 "GCC"
	#define GEN_COMPILER_STRING	 GEN_COMPILER_NAME " compiler, version " INTERNAL_STRINGIFY(__GNUC__) "." INTERNAL_STRINGIFY(__GNUC_MINOR__)

#elif defined(_MSC_VER) // TODO: Validate everything is properly being set for MSVC
	#define GEN_COMPILER_MSVC	   1
	#define GEN_COMPILER_MICROSOFT 1
	#define GEN_COMPILER_VERSION   _MSC_VER
	#define GEN_COMPILER_NAME	   "Microsoft Visual C++"
	#define GEN_COMPILER_STRING	   GEN_COMPILER_NAME " compiler, version " INTERNAL_STRINGIFY(_MSC_VER)

	#if defined(__clang__)
		// Clang's MSVC-compatible driver.
		#define GEN_COMPILER_CLANG_CL 1
	#endif

#endif
