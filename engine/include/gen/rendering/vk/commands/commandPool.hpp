// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"

#include <vulkan/vulkan.hpp>

#include <iosfwd>
#include <thread>

namespace gen
{
	class CommandPool
	{
	public:
		explicit CommandPool(std::string name, const std::thread::id & threadId = std::this_thread::get_id());
		~CommandPool();

		CommandPool(CommandPool const &)				 = delete;
		CommandPool(CommandPool &&) noexcept			 = default;
		CommandPool & operator=(CommandPool const &)	 = delete;
		CommandPool & operator=(CommandPool &&) noexcept = default;

		[[nodiscard]] vk::CommandPool getHandle() const { return m_handle.get(); }

		[[nodiscard]] const std::thread::id & getThreadId() const { return m_threadId; }


	private:
		vk::UniqueCommandPool m_handle{nullptr};
		std::thread::id m_threadId;
		std::string m_name;
	};
} // namespace gen