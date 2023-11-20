// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/graphics/commandBuffer.hpp"

#include "gen/graphics/graphicsExceptions.hpp"

namespace gen
{
	CommandBuffer::CommandBuffer()
	{
		auto device	  = Device::self().getDevice();
		m_commandPool = device.createCommandPoolUnique({vk::CommandPoolCreateFlagBits::eTransient});

		auto const commandBufferAllocateInfo = vk::CommandBufferAllocateInfo{*m_commandPool, vk::CommandBufferLevel::ePrimary, 1};

		if (device.allocateCommandBuffers(&commandBufferAllocateInfo, &m_commandBuffer) != vk::Result::eSuccess)
		{
			throw vulkan_error("Failed to allocate command buffer");
		}

		m_commandBuffer.begin({vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
	}

	void CommandBuffer::submit()
	{
		if (!m_commandBuffer) { return; }

		m_commandBuffer.end();

		auto const cBufferSubmitInfo		 = vk::CommandBufferSubmitInfo{m_commandBuffer};
		auto submitInfoTwo					 = vk::SubmitInfo2{};
		submitInfoTwo.commandBufferInfoCount = 1;
		submitInfoTwo.pCommandBufferInfos	 = &cBufferSubmitInfo;
		auto & device						 = Device::self();
		auto fence							 = device.getDevice().createFenceUnique({});
		device.submit(submitInfoTwo, *fence);
		device.waitForFence(*fence);
		m_commandBuffer = vk::CommandBuffer{};
	}
} // namespace gen