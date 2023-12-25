// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/core/monoInstance.hpp"
#include "gen/rendering/vk/queue.hpp"
#include "gen/system/types.hpp"

#include "gen/rendering/vk/devices/physicalDevice.hpp"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

#include <memory>
#include <optional>
#include <span>
#include <string>

namespace gen
{

	struct DeviceInfo
	{
		std::string name{};
		// Gpu gpu{};
		vk::DeviceSize totalDeviceLocalMemory{0};
		std::vector<vk::ExtensionProperties> extensions{};
	};

	class Device : public MonoInstance<Device>
	{
	public:
		explicit Device(
			Instance & instance,
			PhysicalDevice & gpu,
			std::span<const char * const> requiredExtensions	= {},
			const vk::PhysicalDeviceFeatures & requiredFeatures = {},
			const vk::PhysicalDeviceFeatures & optionalFeatures = {},
			std::optional<u32> selectSpecificGpuIndex			= std::nullopt);

		~Device();

		Device(Device const &)			   = delete;
		Device(Device &&) noexcept		   = default;
		Device & operator=(Device const &) = delete;
		Device & operator=(Device &&)	   = delete;

		[[nodiscard]] vk::Device const & getHandle() const { return m_gpu.get(); }

		[[nodiscard]] PhysicalDevice & getPhysicalDevice() const { return *m_physicalDevice; }

		VmaAllocator getAllocator() const { return m_allocator; }

		vk::Queue const & getQueue(u32 queueFamilyIndex, u32 queueIndex) const;

		vk::Queue const & getQueueByFlags(vk::QueueFlags flags, u32 queueIndex) const;

		vk::Queue const & getQueueByPresentSupport(u32 queueIndex) const;

		vk::Queue const & getIdealGraphicsQueue() const;

		bool isExtensionSupported(std::string_view const & extensionName) const;

		bool isExtensionEnabled(std::string_view const & extensionName) const;

		u32 getQueueFamilyIndex(vk::QueueFlagBits flags) const;

		vk::CommandPool getCommandPool() const;

		std::pair<vk::Image, vk::DeviceMemory> createImage(
			vk::Format format, vk::Extent2D const & extent, u32 mipLevels, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties) const;

		void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::Queue queue, vk::BufferCopy * copyRegion = nullptr) const;

		vk::CommandBuffer createCommandBuffer(vk::CommandBufferLevel level, bool begin = false) const;

		void flushCommandBuffer(vk::CommandBuffer commandBuffer, vk::Queue queue, bool free = true, vk::Semaphore signalSemaphore = VK_NULL_HANDLE) const;

	private:
		vk::Device m_handle{nullptr};

		PhysicalDevice m_gpu{nullptr};

		vk::SurfaceKHR m_surface{nullptr};

		std::vector<vk::ExtensionProperties> m_deviceExtensions{};

		std::vector<const char *> m_enabledExtensions{};

		VmaAllocator m_allocator{VK_NULL_HANDLE};

		// TODO: Maybe add multiple queues at a later date. Unlikely, but will leave this todo for a reminder.
		Queue m_graphicsQueue;

		vk::CommandPool m_commandPool{nullptr};
	};
} // namespace gen