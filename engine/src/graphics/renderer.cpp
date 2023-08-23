// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/renderer.hpp"
#include "gen/graphics/graphicsExceptions.hpp"
#include "gen/graphics/vkHelpers.hpp"

#ifndef GLFW_INCLUDE_NONE
	#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

namespace gen
{

	Renderer::Renderer(const Window & window, std::string const & appName)
		: m_device{appName, appName, VK_API_VERSION_1_2, window}, m_swapchain{window, m_device, m_device.getSurface()} {};

	Renderer::~Renderer()
	{
	}

} // namespace gen