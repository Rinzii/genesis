// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/rendering/vk/semaphore.hpp"

#include <vulkan/vulkan.hpp>

#include <memory>
#include <optional>
#include <vector>

namespace gen
{
	struct SwapchainProperties
	{
		vk::SurfaceFormatKHR surfaceFormat;
		vk::PresentModeKHR presentMode;
		vk::Extent2D extent;
	};

	class Swapchain
	{
	public:
		Swapchain();
		~Swapchain();

		Swapchain(Swapchain const &)			 = delete;
		Swapchain(Swapchain &&) noexcept		 = default;
		Swapchain & operator=(Swapchain const &) = delete;
		Swapchain & operator=(Swapchain &&)		 = delete;

	private:
		vk::UniqueSwapchainKHR m_handle{nullptr};
		vk::UniqueSurfaceKHR m_surface{nullptr};
		std::optional<vk::SurfaceFormatKHR> m_surfaceCapabilities{};
		std::vector<vk::UniqueImage> m_images{};
		std::vector<vk::UniqueImageView> m_imageViews{};
		vk::Extent2D m_extent{};
		std::unique_ptr<Semaphore> m_imageAvailableSemaphore{};
		bool m_vsyncEnabled{false};
	};
} // namespace gen