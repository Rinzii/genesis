// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/utils/exceptions.hpp"

#include "gen/logger/log.hpp"

namespace gen
{
	VulkanException::VulkanException(const std::string & message) : std::runtime_error{message}
	{
		Logger const log{"vulkan"};
		log.error("{}", message);
	}

	GraphicsException::GraphicsException(const std::string & message) : runtime_error(message)
	{
		Logger const log{"graphics"};
		log.error("{}", message);
	}
} // namespace gen