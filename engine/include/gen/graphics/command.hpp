// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "gen/core.hpp"
#include "gen/core/monoInstance.hpp"
#include "gen/graphics/device.hpp"
#include "gen/graphics/swapchain.hpp"
#include "gen/windowing/window.hpp"

// external
#include <vulkan/vulkan.hpp>

// std
#include <vector>

namespace gen
{
	class CommandBufferPool
	{
	public:
		CommandBufferPool(const Device & device);
		~CommandBufferPool() = default;

		CommandBufferPool(const CommandBufferPool &)			 = delete;
		CommandBufferPool(CommandBufferPool &&)					 = delete;
		CommandBufferPool & operator=(const CommandBufferPool &) = delete;
		CommandBufferPool & operator=(CommandBufferPool &&)		 = delete;

		/// Getters
		GEN_NODISCARD vk::CommandBuffer getCommandBuffer() const { return m_commandBuffer; }
		GEN_NODISCARD vk::CommandPool getCommandPool() const { return m_commandPool.get(); }

	private:
		void createCommandPoolAndBuffer(Device const & device);

		vk::UniqueCommandPool m_commandPool{};
		vk::CommandBuffer m_commandBuffer{};
	};
} // namespace gen