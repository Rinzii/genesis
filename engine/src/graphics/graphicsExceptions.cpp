// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/graphicsExceptions.hpp"

#include "logger/log.hpp"

VulkanException::VulkanException(const char * message)
	: m_message(message)
{
	gen::logger::error("vulkan", message);
}

const char * VulkanException::what() const noexcept
{
	return m_message.c_str();
}

