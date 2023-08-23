// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/core.hpp"

// clang-format off

#if GEN_PLATFORM_WINDOWS

#include "gen/system/win32/preWinapi.hpp"

	// We enable STRICT mode to avoid some type conversions in the Win32 API
// https://learn.microsoft.com/en-us/windows/win32/winprog/enabling-strict
#ifndef STRICT
#define STRICT
#endif

#include "gen/system/win32/minWindows.hpp"
#include "gen/system/win32/postWinapi.hpp"

#endif // GEN_PLATFORM_WINDOWS