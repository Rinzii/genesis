// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/rendering/renderFrame.hpp"
#include "gen/rendering/vk/swapchain.hpp"

#include <vulkan/vulkan.hpp>

#include <memory>
#include <span>

namespace gen
{
	class RenderContext
	{

	public:
		RenderContext(
			vk::SurfaceKHR surface,
			vk::PresentModeKHR presentMode,
			const std::span<vk::PresentModeKHR> & presentModePriorityList,
			const std::span<vk::SurfaceFormatKHR> & surfaceFormatPriorityList);

	private:
		vk::Queue m_graphicsQueue;

		std::unique_ptr<Swapchain> m_swapchain;

		SwapchainProperties m_swapchainProperties;

		std::vector<std::unique_ptr<RenderFrame>> m_renderFrames;
	};
} // namespace gen