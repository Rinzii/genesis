// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

// internal
#include "gen/core.hpp"
#include "gen/core/monoInstance.hpp"
#include "gen/windowing/window.hpp"

// external
#include <vulkan/vulkan.hpp>

// std
#include <vector>

namespace gen
{
	struct Gpu
	{
		vk::PhysicalDevice physicalDevice{};
		vk::PhysicalDeviceProperties properties{};
		vk::PhysicalDeviceMemoryProperties memoryProperties{};
		vk::PhysicalDeviceFeatures deviceFeatures{};
		u32 queueFamily{};
	};

	class Device : public MonoInstance<Device>
	{
	public:
		explicit Device(vk::Instance instance);
		~Device();

		Device(const Device &)			   = delete;
		Device(Device &&)				   = delete;
		Device & operator=(const Device &) = delete;
		Device & operator=(Device &&)	   = delete;

		/// Getters

		GEN_NODISCARD vk::SurfaceKHR getSurface() const { return m_surface.get(); }
		GEN_NODISCARD vk::PhysicalDevice getPhysicalDevice() const { return m_gpu.physicalDevice; }
		GEN_NODISCARD vk::Device getDevice() const { return m_device.get(); }
		GEN_NODISCARD vk::Queue getQueue() const { return m_queue; }
		GEN_NODISCARD Gpu getGpu() const { return m_gpu; }
		GEN_NODISCARD vk::PhysicalDeviceProperties getProperties() const { return m_gpu.properties; }
		GEN_NODISCARD u32 getQueueFamily() const { return m_gpu.queueFamily; }

		GEN_NODISCARD u32 getMemoryType(u32 typeBits, vk::MemoryPropertyFlags properties, vk::Bool32 * memTypeFound = nullptr) const;

		/// Helpers

		GEN_NODISCARD bool waitForFence(vk::Fence fence, u64 timeout = std::numeric_limits<u64>::max()) const;
		bool submit(vk::SubmitInfo const & submitInfo, vk::Fence signal = VK_NULL_HANDLE) const;
		bool submit2(vk::SubmitInfo2 const & submitInfo, vk::Fence signal = {}) const;
		bool submitAndPresent(vk::SubmitInfo const & submitInfo, vk::Fence signal, vk::PresentInfoKHR const & presentInfo) const;
		bool submit2AndPresent(vk::SubmitInfo2 const & submitInfo, vk::Fence signal, vk::PresentInfoKHR const & presentInfo) const;

	private:
		void createSurface(vk::Instance instance);
		void selectPhysicalDevice(vk::Instance instance);
		void createLogicalDevice();

		/// Helpers

		static u32 findQueueFamily(vk::PhysicalDevice const & physicalDevice, vk::SurfaceKHR const & surface);

		vk::UniqueSurfaceKHR m_surface{};
		vk::UniqueDevice m_device{};
		vk::Queue m_queue{};

		Gpu m_gpu{};

		mutable std::mutex m_queueMutex{};
		Logger m_logger{"graphics"};
	};
} // namespace gen
