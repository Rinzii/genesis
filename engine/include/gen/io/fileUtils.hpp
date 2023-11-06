// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <filesystem>

namespace gen::io
{
	/**
	 * Get the absolute path of the current active executable.
	 * @return Absolute path to the executable or on failure returns an empty path.
	 */
	std::filesystem::path getExecutablePath();
} // namespace gen::io
