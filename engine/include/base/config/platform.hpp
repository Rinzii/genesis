// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// clang-format off

#if (defined(GEN_PLATFORM_WINDOWS) || (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))) && !defined(CS_UNDEFINED_STRING)
	#undef GEN_PLATFORM_WINDOWS
	#define GEN_PLATFORM_WINDOWS 1
	#define GEN_PLATFORM_NAME "Windows"

	#ifdef _WIN64 // VC++ defines both _WIN32 and _WIN64 when compiling for Win64.
		#define GEN_PLATFORM_WIN64 1
	#else
		#define GEN_PLATFORM_WIN32 1
	#endif

	#define GEN_PLATFORM_DESKTOP 1
	#define GEN_PLATFORM_MICROSOFT 1

	// WINAPI_FAMILY defines to support Windows 8 Metro Apps - mirroring winapifamily.h in the Windows 8 SDK
	#define GEN_WINAPI_FAMILY_APP 1000
	#define GEN_WINAPI_FAMILY_DESKTOP_APP 1001
	#define GEN_WINAPI_FAMILY_GAMES 1006

	#if defined(WINAPI_FAMILY)
		#if defined(_MSC_VER)
			#pragma warning(push, 0)
		#endif
		#include <winapifamily.h>
		#if defined(_MSC_VER)
			#pragma warning(pop)
		#endif
		#if defined(WINAPI_FAMILY_DESKTOP_APP) && WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
			#define GEN_WINAPI_FAMILY GEN_WINAPI_FAMILY_DESKTOP_APP
		#elif defined(WINAPI_FAMILY_APP) && WINAPI_FAMILY == WINAPI_FAMILY_APP
			#define GEN_WINAPI_FAMILY GEN_WINAPI_FAMILY_APP
		#elif defined(WINAPI_FAMILY_GAMES) && WINAPI_FAMILY == WINAPI_FAMILY_GAMES
			#define GEN_WINAPI_FAMILY GEN_WINAPI_FAMILY_GAMES
		#else
			#error Unsupported WINAPI_FAMILY
		#endif
	#else
		#define GEN_WINAPI_FAMILY GEN_WINAPI_FAMILY_DESKTOP_APP
	#endif

	#define GEN_WINAPI_PARTITION_DESKTOP 1
	#define GEN_WINAPI_PARTITION_APP 1
	#define GEN_WINAPI_PARTITION_GAMES (GEN_WINAPI_FAMILY == GEN_WINAPI_FAMILY_GAMES)

	#define GEN_WINAPI_FAMILY_PARTITION(Partition) (Partition)

	// GEN_PLATFORM_WINRT
	// This is a subset of Windows which is used for tablets and the "Metro" (restricted) Windows user interface.
	// WinRT doesn't have access to the Windows "desktop" API, but WinRT can nevertheless run on
	// desktop computers in addition to tablets. The Windows Phone API is a subset of WinRT and is not included
	// in it due to it being only a part of the API.
	#if defined(__cplusplus_winrt)
		#define GEN_PLATFORM_WINRT 1
	#endif

#elif defined(__APPLE__) && __APPLE__
	#include <TargetConditionals.h>

	// Apple family of operating systems.
	#define GEN_PLATFORM_APPLE
	#define GEN_PLATFORM_POSIX 1

	// iPhone
	// TARGET_OS_IPHONE will be undefined on an unknown compiler, and will be defined on gcc.
	#if defined(GEN_PLATFORM_IPHONE) || defined(__IPHONE__) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) ||                                             \
		(defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
		#undef GEN_PLATFORM_IPHONE
		#define GEN_PLATFORM_IPHONE 1
		#define GEN_PLATFORM_NAME "iPhone"
		#define GEN_POSIX_THREADS_AVAILABLE 1

		#define GEN_PLATFORM_MOBILE 1

	// Macintosh OSX
	// __i386__ and __intel__ are defined by the GCC compiler.
	// __MACH__ is defined by the GCC compiler.
	#elif defined(GEN_PLATFORM_OSX) || defined(__MACH__)
		#undef GEN_PLATFORM_OSX
		#define GEN_PLATFORM_OSX 1
		#define GEN_PLATFORM_UNIX 1
		#define GEN_PLATFORM_POSIX 1
		#define GEN_PLATFORM_NAME "OSX"

		#define GEN_PLATFORM_DESKTOP 1
	#else
		#error Unknown Apple Platform
	#endif

#elif defined(GEN_PLATFORM_LINUX) || (defined(__linux) || defined(__linux__))
	#undef GEN_PLATFORM_LINUX
	#define GEN_PLATFORM_LINUX 1
	#define GEN_PLATFORM_UNIX 1
	#define GEN_PLATFORM_POSIX 1
	#define GEN_PLATFORM_NAME "Linux"

	#define GEN_PLATFORM_DESKTOP 1

#elif defined(GEN_PLATFORM_ANDROID) || defined(__ANDROID__)
	#undef GEN_PLATFORM_ANDROID
	#define GEN_PLATFORM_ANDROID 1
	#define GEN_PLATFORM_LINUX 1
	#define GEN_PLATFORM_UNIX 1
	#define GEN_PLATFORM_POSIX 1

	#define GEN_PLATFORM_MOBILE 1

#else
	#error Unknown platform
#endif
