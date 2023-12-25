// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once
/*

#include "gen/core/monoInstance.hpp"
#include "gen/rendering/vk/queue.hpp"
#include "gen/system/types.hpp"

#include "gen/rendering/vk/devices/physicalDevice.hpp"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

#include <optional>
#include <span>
#include <string>

namespace gen
{
	struct Gpu
	{
		vk::PhysicalDevice physicalDevice{ nullptr };
		vk::PhysicalDeviceType type{ vk::PhysicalDeviceType::eOther };
		vk::PhysicalDeviceProperties properties{};
		vk::PhysicalDeviceFeatures features{};
		vk::PhysicalDeviceMemoryProperties memoryProperties{};
		std::vector<vk::QueueFamilyProperties> queueFamilyProperties{};
		u32 graphicsQueueFamilyIndex{ 0 };
		bool presentSupport{ false };
	};



	struct DeviceInfo
	{
		std::string name{};
		Gpu gpu{};
		vk::DeviceSize totalDeviceLocalMemory{0};
		std::vector<vk::ExtensionProperties> extensions{};


	};

	class Device
	{
	public:
		explicit Device(PhysicalDevice & gpu,
						std::span<const char * const> requiredExtensions = {},
						const vk::PhysicalDeviceFeatures & requiredFeatures = {},
						const vk::PhysicalDeviceFeatures & optionalFeatures = {},
						std::optional<u32> selectSpecificGpuIndex = std::nullopt);

		~Device();

		Device(Device const &)			   = delete;
		Device(Device &&) noexcept		   = default;
		Device & operator=(Device const &) = delete;
		Device & operator=(Device &&)	   = delete;


		PhysicalDevice & getPhysicalDevice() { return m_gpu; }








		[[nodiscard]] vk::Device getHandle() const { return m_handle.get(); }

		static vk::PhysicalDevice selectBestPhysicalDevice(std::vector<DeviceInfo> & gpus,
													vk::SurfaceKHR surface,
													std::span<const char * const> requiredExtensions = {},
													const vk::PhysicalDeviceFeatures & requiredFeatures = {});

		static vk::PhysicalDevice selectBestPhysicalDevice(const vk::Instance & instance,
													const vk::SurfaceKHR & surface,
													std::span<const char * const> requiredExtensions = {},
													const vk::PhysicalDeviceFeatures & requiredFeatures = {});

		[[nodiscard]] vk::SurfaceKHR getSurface() const { return m_surface; }

		[[nodiscard]] const vk::Queue & getGraphicsQueue() const { return m_graphicsQueue; }

		u32 getGraphicsQueueFamily() const { return m_graphicsQueueFamily; }

		[[nodiscard]] vk::PhysicalDevice getPhysicalDevice() const { return m_selectedGpu.physicalDevice; }

		[[nodiscard]] vk::DeviceSize getTotalDeviceMemory() const { return totalDeviceMemory; }

		[[nodiscard]] VmaAllocator getAllocator() const { return m_allocator; }

		[[nodiscard]] Gpu getSelectedGpu() const { return m_selectedGpu; }

	private:

		static Gpu selectPhysicalDevice(const vk::Instance & instance,
											const vk::SurfaceKHR & surface,
											std::span<const char * const> requiredExtensions = {},
											const vk::PhysicalDeviceFeatures & requiredFeatures = {},
											const vk::PhysicalDeviceFeatures & optionalFeatures = {});



	private:


		vk::UniqueDevice m_handle{nullptr};

		VmaAllocator m_allocator{VK_NULL_HANDLE};

		vk::SurfaceKHR m_surface{nullptr};

		vk::DeviceSize totalDeviceMemory{0};

		vk::Queue m_graphicsQueue{nullptr};

		u32 m_graphicsQueueFamily{0};

		Gpu m_selectedGpu{};

		DeviceInfo m_gpu{};

		std::vector<DeviceInfo> m_gpus{};

	};
} // namespace gen

 */