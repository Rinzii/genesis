// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <stdexcept>
#include <string>

namespace gen
{
	struct VulkanException : public std::runtime_error
	{

		explicit VulkanException(const std::string & message);
	};

	struct GraphicsException : public std::runtime_error
	{

		explicit GraphicsException(const std::string & message);
	};

} // namespace gen