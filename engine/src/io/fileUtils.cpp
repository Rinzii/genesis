// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/io/fileUtils.hpp"

#include "gen/core/base/config/platform.hpp"

#ifdef GEN_PLATFORM_WINDOWS
	#include "gen/system/win32/windows.hpp"
#elif defined(GEN_PLATFORM_LINUX) || defined(GEN_PLATFORM_APPLE)
	#include <unistd.h>
#endif

namespace gen::io
{
	static constexpr std::size_t maxPathSize_v{256};

#ifdef GEN_PLATFORM_WINDOWS
	std::filesystem::path getExecutablePath()
	{
		char buffer[maxPathSize_v];
		DWORD length = GetModuleFileNameA(nullptr, buffer, maxPathSize_v);

		if (length == 0) { return std::filesystem::path(); }

		return std::filesystem::path(buffer).parent_path();
	}
#elif defined(GEN_PLATFORM_LINUX) || defined(GEN_PLATFORM_APPLE)
	std::filesystem::path getExecutablePath()
	{
		char buffer[maxPathSize_v];														// NOLINT
		ssize_t const outputtedLen = readlink("/proc/self/exe", buffer, maxPathSize_v); // NOLINT
		if (outputtedLen != -1)
		{
			// NOLINTNEXTLINE
			buffer[outputtedLen] = '\0'; // Null-terminate the string
			return std::filesystem::path(buffer).parent_path();
		}

		return {};
	}
#else
	std::filesystem::path getExecutablePath()
	{
		// If for some reason we are on an unsupported platform, just return an empty path.
		return {};
	}
#endif
} // namespace gen::io