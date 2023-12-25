// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <vulkan/vulkan.hpp>

#include "gen/rendering/vk/descriptors/descriptorSet.hpp"

// #include <optional>

namespace gen
{
	class Buffer
	{
	public:
		enum class State
		{
			eReset,
			eChanged,
			eNormal,
		};

		Buffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, const void * data = nullptr);
		virtual ~Buffer();

		void mapMemory(vk::DeviceSize size, vk::DeviceSize offset = 0);
		void unmapMemory();

		[[nodiscard]] vk::DeviceSize getSize() const { return m_size; }
		[[nodiscard]] const vk::Buffer & getBuffer() const { return m_buffer; }
		[[nodiscard]] const vk::DeviceMemory & getMemory() const { return m_memory; }

		static uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags & requiredProperties);

		static void insertBufferMemoryBarrier(
			vk::CommandBuffer commandBuffer,
			vk::Buffer buffer,
			vk::AccessFlags srcAccessMask,
			vk::AccessFlags dstAccessMask,
			vk::PipelineStageFlags srcStageMask,
			vk::PipelineStageFlags dstStageMask,
			uint32_t srcQueueFamilyIndex,
			uint32_t dstQueueFamilyIndex,
			vk::DeviceSize offset,
			vk::DeviceSize size);

	protected:
		vk::DeviceSize m_size;
		vk::Buffer m_buffer		  = nullptr;
		vk::DeviceMemory m_memory = nullptr;
	};

} // namespace gen