// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// Use this for reference: https://en.cppreference.com/w/cpp/compiler_support

#include "base/config/compiler.hpp"

// clang-format off

// https://learn.microsoft.com/en-us/cpp/intrinsics/compiler-intrinsics?view=msvc-170
// https://clang.llvm.org/docs/LanguageExtensions.html
// https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/X86-Built-in-Functions.html

// ------------------------------------------------------------------------
// GEN_STRINGIFY
//
// Example usage:
//     printf("Line: %s", GEN_STRINGIFY(__LINE__));
//
#ifndef GEN_STRINGIFY
#define GEN_STRINGIFY(x) GEN_STRINGIFYIMPL(x)
#define GEN_STRINGIFYIMPL(x) #x
#endif

// https://gcc.gnu.org/onlinedocs/cpp/Pragmas.html
// https://clang.llvm.org/docs/UsersManual.html#controlling-diagnostics-via-pragmas
#ifndef GEN_WARNING
    #if defined(GEN_COMPILER_CLANG) || defined(GEN_COMPILER_GCC)
        #define GEN_WARNING(msg) _Pragma(GEN_STRINGIFY(GCC warning msg)
    #else
        #define GEN_WARNING(msg)
    #endif
#endif



#ifndef GEN_NODISCARD
#if defined(GEN_COMPILER_CPP17_ENABLED)
#define GEN_NODISCARD [[nodiscard]]
#else
#if defined(GEN_COMPILER_MSVC)
#include <sal.h>
			#define GEN_NODISCARD __checkReturn
#elif defined(GEN_COMPILER_GCC) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 701) || defined(GEN_COMPILER_CLANG)
// Check that GNUC is at least 7.1
			// This is required as GNUC 7.0 has a bug where it will not warn if the return value is not used
			#define GEN_NODISCARD __attribute__((warn_unused_result))
#else
#define GEN_NODISCARD
#endif
#endif
#endif

#ifndef GEN_FORCE_INLINE
#ifndef GEN_DISABLE_FORCED_INLINING
#if defined(GEN_COMPILER_MSVC)
#define GEN_FORCE_INLINE __forceinline
#elif defined(GEN_COMPILER_GCC) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 301) || defined(GEN_COMPILER_CLANG)
#if defined(__cplusplus)
				#define GEN_FORCE_INLINE inline __attribute__((always_inline))
			#else
				#define GEN_FORCE_INLINE __inline__ __attribute__((always_inline))
			#endif
#else
#if defined(__cplusplus)
#define GEN_FORCE_INLINE inline
#else
#define GEN_FORCE_INLINE __inline
#endif
#endif
#else // If GEN_DISABLE_FORCED_INLINING is defined then don't force inlining
#if defined(__cplusplus)
			#define GEN_FORCE_INLINE inline
		#else
			#define GEN_FORCE_INLINE __inline
		#endif
#endif
#endif

#ifndef GEN_NOINLINE
#if defined(GEN_COMPILER_MSVC)
#define GEN_NOINLINE __declspec(noinline)
#elif defined(GEN_COMPILER_GCC) || defined(GEN_COMPILER_CLANG)
#define GEN_NOINLINE __attribute__((noinline))
#else
#define GEN_NOINLINE
#endif
#endif

#ifndef GEN_DEPRECATED
    #define GEN_DEPRECATED [[deprecated]]
#endif

#ifndef GEN_DEPRECATED_MESSAGE
    #define GEN_DEPRECATED_MESSAGE(message) [[deprecated(message)]]
#endif

#ifndef GEN_FALLTHROUGH
#if defined(GEN_COMPILER_CPP17_ENABLED)
#define GEN_FALLTHROUGH [[fallthrough]]
#elif defined(GEN_COMPILER_GCC) || defined(GEN_COMPILER_CLANG)
#define GEN_FALLTHROUGH __attribute__((fallthrough))
#elif defined(GEN_COMPILER_MSVC)
#define GEN_FALLTHROUGH __fallthrough
#else
#define GEN_FALLTHROUGH
#endif
#endif

#if defined(GEN_COMPILER_MSVC)
#define GEN_PRAGMA(x) __pragma(x)
#elif defined(GEN_COMPILER_GCC) || defined(GEN_COMPILER_CLANG)
#define GEN_PRAGMA(x) _Pragma(#x)
#else
#define GEN_PRAGMA(x)
#endif

// ------------------------------------------------------------------------
// GEN_DISABLE_GCC_WARNING / GEN_RESTORE_GCC_WARNING
//
// Example usage:
//     // Only one warning can be ignored per statement, due to how GCC works.
//     GEN_DISABLE_GCC_WARNING(-Wuninitialized)
//     GEN_DISABLE_GCC_WARNING(-Wunused)
//     <code>
//     GEN_RESTORE_GCC_WARNING()
//     GEN_RESTORE_GCC_WARNING()
//
#ifndef GEN_DISABLE_GCC_WARNING
#if defined(GEN_COMPILER_GNUC)
#define GENGCCWHELP0(x) #x
		#define GENGCCWHELP1(x) GENGCCWHELP0(GCC diagnostic ignored x)
		#define GENGCCWHELP2(x) GENGCCWHELP1(#x)
#endif

#if defined(GEN_COMPILER_GNUC) && (GEN_COMPILER_VERSION >= 4006) // Can't test directly for __GNUC__ because some compilers lie.
#define GEN_DISABLE_GCC_WARNING(w) _Pragma("GCC diagnostic push") _Pragma(GENGCCWHELP2(w))
#else
#define GEN_DISABLE_GCC_WARNING(w)
#endif
#endif

#ifndef GEN_RESTORE_GCC_WARNING
#if defined(GEN_COMPILER_GNUC) && (GEN_COMPILER_VERSION >= 4006)
#define GEN_RESTORE_GCC_WARNING() _Pragma("GCC diagnostic pop")
#else
#define GEN_RESTORE_GCC_WARNING()
#endif
#endif

// ------------------------------------------------------------------------
// GEN_ENABLE_GCC_WARNING_AS_ERROR / GEN_DISABLE_GCC_WARNING_AS_ERROR
//
// Example usage:
//     // Only one warning can be treated as an error per statement, due to how GCC works.
//     GEN_ENABLE_GCC_WARNING_AS_ERROR(-Wuninitialized)
//     GEN_ENABLE_GCC_WARNING_AS_ERROR(-Wunused)
//     <code>
//     GEN_DISABLE_GCC_WARNING_AS_ERROR()
//     GEN_DISABLE_GCC_WARNING_AS_ERROR()
//
#ifndef GEN_ENABLE_GCC_WARNING_AS_ERROR
#if defined(GEN_COMPILER_GNUC)
#define GENGCCWERRORHELP0(x) #x
		#define GENGCCWERRORHELP1(x) GENGCCWERRORHELP0(GCC diagnostic error x)
		#define GENGCCWERRORHELP2(x) GENGCCWERRORHELP1(#x)
#endif
#endif

#if defined(GEN_COMPILER_GNUC) && (GEN_COMPILER_VERSION >= 4006) // Can't test directly for __GNUC__ because some compilers lie.
#define GEN_ENABLE_GCC_WARNING_AS_ERROR(w) _Pragma("GCC diagnostic push") _Pragma(GENGCCWERRORHELP2(w))
#else
#define GEN_ENABLE_GCC_WARNING_AS_ERROR(w)
#endif

#ifndef GEN_DISABLE_GCC_WARNING_AS_ERROR
#if defined(GEN_COMPILER_GNUC) && (GEN_COMPILER_VERSION >= 4006)
#define GEN_DISABLE_GCC_WARNING_AS_ERROR() _Pragma("GCC diagnostic pop")
#else
#define GEN_DISABLE_GCC_WARNING_AS_ERROR()
#endif
#endif

#ifndef GEN_ENABLE_IGNORE_GCC_WARNING
#if defined(GEN_COMPILER_GNUC)
#define GENGCCIGNOREHELP0(x) #x
		#define GENGCCIGNOREHELP1(x) GENGCCIGNOREHELP0(GCC diagnostic ignored x)
		#define GENGCCIGNOREHELP2(x) GENGCCIGNOREHELP1(#x)
#endif
#endif

#ifndef GEN_ENABLE_IGNORE_GCC_WARNING
#if defined(GEN_COMPILER_GNUC) && (GEN_COMPILER_VERSION >= 4006) // Can't test directly for __GNUC__ because some compilers lie.
#define GEN_ENABLE_IGNORE_GCC_WARNING(w) _Pragma("GCC diagnostic push") _Pragma(GENGCCIGNOREHELP2(w))
#else
#define GEN_ENABLE_IGNORE_GCC_WARNING(w)
#endif
#endif

#ifndef GEN_DISABLE_IGNORE_GCC_WARNING
#if defined(GEN_COMPILER_GNUC)
#define GENGCCIGNOREDISABLEHELP0(x) #x
		#define GENGCCIGNOREDISABLEHELP1(x) GENGCCIGNOREDISABLEHELP0(GCC diagnostic warning x)
		#define GENGCCIGNOREDISABLEHELP2(x) GENGCCIGNOREDISABLEHELP1(#x)
#endif
#endif

#ifndef GEN_DISABLE_IGNORE_GCC_WARNING
#if defined(GEN_COMPILER_GNUC) && (GEN_COMPILER_VERSION >= 4004)
#define GEN_DISABLE_IGNORE_GCC_WARNING(w) _Pragma(GENGCCIGNOREDISABLEHELP2(w)) _Pragma("GCC diagnostic pop")
#else
#define GEN_DISABLE_IGNORE_GCC_WARNING(w)
#endif
#endif

// ------------------------------------------------------------------------
// GEN_DISABLE_CLANG_WARNING / GEN_RESTORE_CLANG_WARNING
//
// Example usage:
//     // Only one warning can be ignored per statement, due to how clang works.
//     GEN_DISABLE_CLANG_WARNING(-Wuninitialized)
//     GEN_DISABLE_CLANG_WARNING(-Wunused)
//     <code>
//     GEN_RESTORE_CLANG_WARNING()
//     GEN_RESTORE_CLANG_WARNING()
//
#ifndef GEN_DISABLE_CLANG_WARNING
#if defined(GEN_COMPILER_CLANG) || defined(GEN_COMPILER_CLANG_CL)
#define GENCLANGWHELP0(x) #x
		#define GENCLANGWHELP1(x) GENCLANGWHELP0(clang diagnostic ignored x)
		#define GENCLANGWHELP2(x) GENCLANGWHELP1(#x)

		#define GEN_DISABLE_CLANG_WARNING(w) _Pragma("clang diagnostic push") _Pragma(GENCLANGWHELP2(-Wunknown-warning-option)) _Pragma(GENCLANGWHELP2(w))
#else
#define GEN_DISABLE_CLANG_WARNING(w)
#endif
#endif

#ifndef GEN_RESTORE_CLANG_WARNING
#if defined(GEN_COMPILER_CLANG) || defined(GEN_COMPILER_CLANG_CL)
#define GEN_RESTORE_CLANG_WARNING() _Pragma("clang diagnostic pop")
#else
#define GEN_RESTORE_CLANG_WARNING()
#endif
#endif

// ------------------------------------------------------------------------
// GEN_ENABLE_CLANG_WARNING_AS_ERROR / GEN_DISABLE_CLANG_WARNING_AS_ERROR
//
// Example usage:
//     // Only one warning can be treated as an error per statement, due to how clang works.
//     GEN_ENABLE_CLANG_WARNING_AS_ERROR(-Wuninitialized)
//     GEN_ENABLE_CLANG_WARNING_AS_ERROR(-Wunused)
//     <code>
//     GEN_DISABLE_CLANG_WARNING_AS_ERROR()
//     GEN_DISABLE_CLANG_WARNING_AS_ERROR()
//
#ifndef GEN_ENABLE_CLANG_WARNING_AS_ERROR
#if defined(GEN_COMPILER_CLANG) || defined(GEN_COMPILER_CLANG_CL)
#define GENCLANGWERRORHELP0(x) #x
		#define GENCLANGWERRORHELP1(x) GENCLANGWERRORHELP0(clang diagnostic error x)
		#define GENCLANGWERRORHELP2(x) GENCLANGWERRORHELP1(#x)

		#define GEN_ENABLE_CLANG_WARNING_AS_ERROR(w) _Pragma("clang diagnostic push") _Pragma(GENCLANGWERRORHELP2(w))
#else
#define GEN_DISABLE_CLANG_WARNING(w)
#endif
#endif

#ifndef GEN_DISABLE_CLANG_WARNING_AS_ERROR
#if defined(GEN_COMPILER_CLANG) || defined(GEN_COMPILER_CLANG_CL)
#define GEN_DISABLE_CLANG_WARNING_AS_ERROR() _Pragma("clang diagnostic pop")
#else
#define GEN_DISABLE_CLANG_WARNING_AS_ERROR()
#endif
#endif
