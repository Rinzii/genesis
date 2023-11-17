// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"

#include "gen/rendering/vk/queue.hpp"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

namespace gen
{
	struct Gpu
	{
		vk::PhysicalDevice physicalDevice;
		vk::PhysicalDeviceProperties properties;
		vk::PhysicalDeviceFeatures features;
		vk::PhysicalDeviceMemoryProperties memoryProperties;
	};

	struct DeviceInfo
	{
		vk::DeviceCreateInfo createInfo;
		vk::PhysicalDeviceFeatures enabledFeatures;
		std::vector<const char *> enabledExtensions;
		std::vector<const char *> enabledLayers;
	};

	class Device
	{
	public:
		Device(std::string name);
		~Device();

		Device(Device const &)			   = delete;
		Device(Device &&) noexcept		   = default;
		Device & operator=(Device const &) = delete;
		Device & operator=(Device &&)	   = delete;

		vk::Device getHandle() const { return m_handle.get(); }

	private:
		vk::UniqueDevice m_handle{nullptr};
		Gpu m_gpu;
		VmaAllocator m_allocator{nullptr};

		// TODO: Maybe handle multiple queues at a later date? Idk maybe not.

		Queue m_graphicsQueue{nullptr};

		u32 m_graphicsQueueFamilyIndex{0};
	};
} // namespace gen