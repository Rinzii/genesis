// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/graphicsExceptions.hpp"

#include "logger/log.hpp"

namespace gen
{
	vulkan_error::vulkan_error(const std::string& message)
	    : std::runtime_error{ message }
	{
		gen::logger::error("vulkan", message);
	}

} // namespace gen


