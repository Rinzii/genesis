// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <stdexcept>
#include <string>

namespace gen
{
	struct vulkan_error : public std::runtime_error
	{

		explicit vulkan_error(const std::string & message);
	};

	struct graphics_error : public std::runtime_error
	{

		explicit graphics_error(const std::string & message);
	};

} // namespace gen
