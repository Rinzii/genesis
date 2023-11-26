// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/graphicsExceptions.hpp"

#include "gen/system/log.hpp"

namespace gen
{
	vulkan_error::vulkan_error(const std::string & message) : std::runtime_error{message}
	{
		Logger const log{"vulkan"};
		log.error("{}", message);
	}

	graphics_error::graphics_error(const std::string & message) : runtime_error(message)
	{
		Logger const log{"graphics"};
		log.error("{}", message);
	}
} // namespace gen
