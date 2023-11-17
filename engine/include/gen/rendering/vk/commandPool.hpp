// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/rendering/vk/commandBuffer.hpp"

#include <vulkan/vulkan.hpp>

#include <memory>
#include <string>

namespace gen
{
	class CommandPool
	{
	public:
		CommandPool(std::string name);
		~CommandPool();

		CommandPool(CommandPool const &)				 = delete;
		CommandPool(CommandPool &&) noexcept			 = default;
		CommandPool & operator=(CommandPool const &)	 = delete;
		CommandPool & operator=(CommandPool &&) noexcept = default;

		vk::CommandPool getHandle() const { return m_handle.get(); }

	private:
		vk::UniqueCommandPool m_handle{nullptr};

		std::vector<std::unique_ptr<CommandBuffer>> m_commandBuffers;
	};
} // namespace gen