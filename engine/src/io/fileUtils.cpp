// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/io/fileUtils.hpp"

#include "gen/core/base/config/platform.hpp"


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

#elif defined(GEN_PLATFORM_LINUX)

#include <unistd.h>
#include <linux/limits.h> // PATH_MAX

namespace gen::io
{
	std::filesystem::path getExecutablePath()
	{
		char buffer[PATH_MAX]; // NOLINT
		// Read the symbolic link pointing to the executable
		ssize_t const outputtedLen = readlink("/proc/self/exe", buffer, PATH_MAX); // NOLINT
		if (outputtedLen != -1)
		{
			// NOLINTNEXTLINE
			buffer[outputtedLen] = '\0'; // Null-terminate the string
			return std::filesystem::path(buffer).parent_path();
		}

		return {};
	}
}
// END GEN_PLATFORM_LINUX

#elif defined(GEN_PLATFORM_APPLE)

	#include <mach-o/dyld.h> // required for _NSGetExecutablePath()
	#include <cstdint>

namespace gen::io
{
	std::filesystem::path getExecutablePath()
	{
		// On mac / ios paths can be no longer than 1024 characters.
		char buffer[1024]; // NOLINT
		std::uint32_t const outputtedLen = 1024;
		if (_NSGetExecutablePath(buffer, &outputtedLen) == 0) // This fails if the buffer size is too small.
		{
			return std::filesystem::path(buffer).parent_path();
		}

		return {};
	}
}
// END GEN_PLATFORM_APPLE
#endif

