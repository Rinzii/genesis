// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/rendering/fencePool.hpp"
#include "gen/rendering/semaphorePool.hpp"
#include "gen/system/types.hpp"

#include <vulkan/vulkan.hpp>

#include <map>

namespace gen
{
	struct FrameSwapchainData
	{
		std::vector<vk::Image> images;
		std::vector<vk::UniqueImageView> imageViews;
	};

	class Frame
	{
	public:
		explicit Frame(FrameSwapchainData && swapchainData, vk::CommandPoolCreateFlagBits commandPoolCreateBits = vk::CommandPoolCreateFlagBits::eTransient);

		explicit Frame(std::vector<vk::Image> && image, vk::CommandPoolCreateFlagBits commandPoolCreateBits = vk::CommandPoolCreateFlagBits::eTransient);

		explicit Frame(
			std::vector<vk::UniqueImageView> && imageView, vk::CommandPoolCreateFlagBits commandPoolCreateBits = vk::CommandPoolCreateFlagBits::eTransient);

		~Frame() = default;

		Frame(const Frame &)			 = delete;
		Frame & operator=(const Frame &) = delete;

		Frame(Frame &&)				= delete;
		Frame & operator=(Frame &&) = delete;

		void reset();

		void updateFrame();

		// Getters

		[[nodiscard]] vk::CommandPool getCommandPool(u32 queueFamilyIndex, u32 queueIndex = 0);

		[[nodiscard]] vk::DescriptorPool getDescriptorPool(u32 queueFamilyIndex, std::size_t hash);

		[[nodiscard]] vk::DescriptorSet getDescriptorSet(u32 queueFamilyIndex, std::size_t hash);

	private:
		vk::Extent2D m_extent;

		vk::UniqueCommandPool m_commandPool;

		vk::CommandPoolCreateFlagBits m_commandPoolCreateBits;

		FrameSwapchainData m_swapchainData;

		vk::Fence m_fence;

		vk::UniqueSemaphore m_semaphore;
	};

} // namespace gen
