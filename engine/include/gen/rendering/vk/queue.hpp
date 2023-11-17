// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"

#include <vulkan/vulkan.hpp>

namespace gen
{
	class CommandBuffer;

	class Queue
	{
	public:
		Queue(u32 familyIndex, u32 queueIndex, vk::QueueFamilyProperties properties, vk::Bool32 canPresent);
		~Queue() = default;
		Queue(const Queue&) = delete;
		Queue(Queue&&) noexcept ;
		Queue& operator=(const Queue&) = delete;
		Queue& operator=(Queue&&) = delete;

		void submit(const CommandBuffer & commandBuffer, vk::Fence fence, std::string name) const;

		vk::Result present(const vk::PresentInfoKHR & presentInfo) const;

		// Getters
		[[nodiscard]] vk::Queue getHandle() const { return m_handle; }
		[[nodiscard]] u32 getFamilyIndex() const { return m_familyIndex; }
		[[nodiscard]] u32 getQueueIndex() const { return m_queueIndex; }
		[[nodiscard]] vk::QueueFamilyProperties getProperties() const { return m_properties; }
		[[nodiscard]] vk::Bool32 canPresent() const { return m_canPresent; }

	private:
		vk::Queue m_handle;
		u32 m_familyIndex{0};
		u32 m_queueIndex{0};
		vk::QueueFamilyProperties m_properties{};
		vk::Bool32 m_canPresent{vk::False};
	};

} // namespace gen