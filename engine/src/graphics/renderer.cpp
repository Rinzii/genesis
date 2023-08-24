// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/renderer.hpp"

#include <vulkan/vulkan.hpp>

namespace gen
{

	Renderer::Renderer(const Window & window, const char * const appName, const u32 appVersion)
		: m_device{appName, appVersion, "Genesis Engine", VK_API_VERSION_1_2, window}, m_swapchain{window, m_device, m_device.getSurface()},
		  m_commandBufferPool{m_device} {};

} // namespace gen