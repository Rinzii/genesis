// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/renderer.hpp"

#include <vulkan/vulkan.hpp>

namespace gen
{

	Renderer::Renderer(const char * const appName, const u32 appVersion)
		: m_device(std::make_unique<Device>(appName, appVersion, "Genesis Engine", VK_API_VERSION_1_2)),
		  m_swapchain(std::make_unique<Swapchain>(Window::getInstance(), *m_device, m_device->getSurface())),
		  m_commandBufferPool(std::make_unique<CommandBufferPool>(*m_device))
	{
		m_logger.info("Renderer created");
	}

} // namespace gen