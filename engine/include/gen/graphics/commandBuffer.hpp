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
	class CommandBuffer
	{
	public:
		CommandBuffer();
		~CommandBuffer() { submit(); }

		CommandBuffer(const CommandBuffer &)			 = delete;
		CommandBuffer(CommandBuffer &&)					 = delete;
		CommandBuffer & operator=(const CommandBuffer &) = delete;
		CommandBuffer & operator=(CommandBuffer &&)		 = delete;

		/// Getters
		GEN_NODISCARD vk::CommandBuffer get() const { return m_commandBuffer; }

		/// Helpers
		void submit();

		explicit operator vk::CommandBuffer() const { return get(); }

	private:
		vk::UniqueCommandPool m_commandPool{};
		vk::CommandBuffer m_commandBuffer{};
	};
} // namespace gen