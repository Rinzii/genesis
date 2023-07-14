

#pragma once

#include "core.hpp"

#if GEN_PLATFORM_WINDOWS

#include "system/win32/preWinapi.hpp"

	// We enable STRICT mode to avoid some type conversions in the Win32 API
// https://learn.microsoft.com/en-us/windows/win32/winprog/enabling-strict
#ifndef STRICT
#define STRICT
#endif

#include "system/win32/postWinapi.hpp"
#include "system/win32/minWindows.hpp"

#endif // GEN_PLATFORM_WINDOWS
