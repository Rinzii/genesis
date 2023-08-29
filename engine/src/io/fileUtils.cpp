// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/io/fileUtils.hpp"

#include "gen/core/base/config/platform.hpp" // GEN_PLATFORM_*


#ifdef GEN_PLATFORM_WINDOWS
#include "gen/system/win32/windows.hpp"

namespace gen::io
{
	std::filesystem::path getExecutablePath()
	{
		wchar_t buffer[MAX_PATH];
		DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);

		if (length != 0)
		{
			return std::filesystem::path(buffer).parent_path();
		}

		return {};
	}
}
// END GEN_PLATFORM_WINDOWS

#elif defined(GEN_PLATFORM_APPLE)

#include <mach-o/dyld.h> // required for _NSGetExecutablePath()
#include <cstdint> // required for std::uint32_t

namespace gen::io
{
	std::filesystem::path getExecutablePath()
	{
		// On mac / ios paths can be no longer than 1024 characters.
		char buffer[1024]; // NOLINT
		std::uint32_t outputtedLen = sizeof(buffer);
		if (_NSGetExecutablePath(buffer, &outputtedLen) == 0) // This fails if the buffer size is too small.
		{
			return std::filesystem::path(buffer).parent_path();
		}

		return {};
	}
}
// END GEN_PLATFORM_APPLE

#else // This should cover linux, unix, and android (does not cover bsd)

namespace gen::io
{
	std::filesystem::path getExecutablePath()
	{
		return std::filesystem::read_symlink("/proc/self/exe").parent_path();
	}
}
#endif

