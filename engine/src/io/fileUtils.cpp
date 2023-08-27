// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/io/fileUtils.hpp"

#include "gen/core/base/config/platform.hpp"

#ifdef GEN_PLATFORM_WINDOWS
	#include "gen/system/win32/windows.hpp"
#elif defined(GEN_PLATFORM_LINUX) || defined(GEN_PLATFORM_APPLE)
	#include <unistd.h>
	#ifdef GEN_PLATFORM_APPLE
		#include <mach-o/dyld.h> // required for _NSGetExecutablePath()
		#include <cstdint>
	#endif
#endif

namespace gen::io
{
	static constexpr std::size_t maxPathSize_v{256};

	std::filesystem::path getExecutablePath()
	{
#ifdef GEN_PLATFORM_WINDOWS
		wchar_t buffer[maxPathSize_v];
		DWORD length = GetModuleFileNameW(nullptr, buffer, maxPathSize_v);

		if (length == 0) { return {}; }

		return std::filesystem::path(buffer).parent_path();
		// END GEN_PLATFORM_WINDOWS

#elif defined(GEN_PLATFORM_LINUX) || defined(GEN_PLATFORM_APPLE)

		char buffer[maxPathSize_v]; // NOLINT

		// Apple is slightly different from normal unix and requires a special function.
	#ifdef GEN_PLATFORM_APPLE
		std::uint32_t const outputtedLen = maxPathSize_v;
		if (_NSGetExecutablePath(buffer, &outputtedLen) == 0) // This fails if the buffer size is too small.
		{
			return std::filesystem::path(buffer).parent_path();
		}
	#elif defined(GEN_PLATFORM_LINUX)
		// Read the symbolic link pointing to the executable
		ssize_t const outputtedLen = readlink("/proc/self/exe", buffer, maxPathSize_v); // NOLINT
		if (outputtedLen != -1)
		{
			// NOLINTNEXTLINE
			buffer[outputtedLen] = '\0'; // Null-terminate the string
			return std::filesystem::path(buffer).parent_path();
		}
	#endif

		// Permission denied (We must be inetd with this app run as other than root).
		return {};
		// END defined(GEN_PLATFORM_LINUX) || defined(GEN_PLATFORM_APPLE)

#else
		// If for some reason we are on an unsupported platform, just return an empty path.
		return {};
// END ELSE
#endif
	}

} // namespace gen::io