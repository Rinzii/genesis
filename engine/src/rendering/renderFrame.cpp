// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/renderFrame.hpp"
#include "gen/rendering/utils/vkHelpers.hpp"
#include "gen/rendering/vk/devices/logicalDevice.hpp"

namespace gen
{
	RenderFrame::RenderFrame(FrameSwapchainData && swapchainData, vk::CommandPoolCreateFlagBits resetFlags) : m_swapchainData{std::move(swapchainData)}
	{
		vk::CommandPoolCreateInfo poolInfo{};
		poolInfo.queueFamilyIndex = Device::self().getQueueFamilyIndex();
		poolInfo.flags			  = resetFlags;
	}

	RenderFrame::RenderFrame(std::vector<vk::Image> && image, vk::CommandPoolCreateFlagBits commandPoolCreateBits)
		: Frame(FrameSwapchainData{std::move(image), {}}, commandPoolCreateBits)
	{
	}

	RenderFrame::RenderFrame(std::vector<vk::UniqueImageView> && imageView, vk::CommandPoolCreateFlagBits commandPoolCreateBits)
		: Frame(FrameSwapchainData{{}, std::move(imageView)}, commandPoolCreateBits)
	{
	}

	void RenderFrame::reset()
	{
		vk::utils::checkResult(Device::self().getHandle().waitForFences(1, &m_fence, vk::True, std::numeric_limits<u32>::max()));
		vk::utils::checkResult(Device::self().getHandle().resetFences(1, &m_fence));

		Device::self().getHandle().resetCommandPool(m_commandPool.get(), vk::CommandPoolResetFlagBits::eReleaseResources);
	}

	void RenderFrame::updateFrame()
	{
	}

	vk::CommandPool RenderFrame::getCommandPool(u32 queueFamilyIndex, u32 queueIndex)
	{
		return vk::CommandPool();
	}

	vk::DescriptorPool RenderFrame::getDescriptorPool(u32 queueFamilyIndex, std::size_t hash)
	{
		return vk::DescriptorPool();
	}

	vk::DescriptorSet RenderFrame::getDescriptorSet(u32 queueFamilyIndex, std::size_t hash)
	{
		return vk::DescriptorSet();
	}

} // namespace gen