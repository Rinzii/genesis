// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/io/fileUtils.hpp"

#include "gen/core.hpp"

#ifdef GEN_PLATFORM_WINDOWS
#include "gen/system/win32/windows.hpp"
#endif

#ifdef GEN_PLATFORM_LINUX
#include <unistd.h>
#endif

namespace gen::io::util
{
	static constexpr std::size_t maxPathSize_v{ 256 };

#ifdef GEN_PLATFORM_WINDOWS
	std::filesystem::path getCurrentExecutableDir()
	{
		char buffer[maxPathSize_v];
		GetModuleFileNameA(nullptr, buffer, maxPathSize_v);
		return std::filesystem::path(buffer).parent_path();
	}
#endif
#ifdef GEN_PLATFORM_LINUX || GEN_PLATFORM_APPLE
	std::filesystem::path getCurrentExecutableDir()
	{
		char buffer[maxPathSize_v];
		ssize_t outputtedLen = readlink("/proc/self/exe", buffer, maxPathSize_v);
		if (outputtedLen != -1)
		{
			return std::filesystem::path(buffer).parent_path();
		}

		return std::filesystem::path();
	}
#endif
}