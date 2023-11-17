// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/core/monoInstance.hpp"
#include "gen/logger/log.hpp"
#include "gen/system/types.hpp"

#include <vulkan/vulkan.hpp>

#include <mutex>

namespace gen
{
	struct Gpu
	{
		vk::PhysicalDevice physicalDevice{nullptr};

		vk::PhysicalDeviceProperties properties{};

		vk::PhysicalDeviceMemoryProperties memoryProperties{};

		vk::PhysicalDeviceFeatures features{};

		vk::QueueFamilyProperties queueFamilyProperties{};

		u32 graphicsQueueFamilyIndex{0};
	};

	class Device : public MonoInstance<Device>
	{
	public:
		explicit Device(vk::Instance instance, const std::vector<const char *> & requiredExtensions = {});

		~Device();

		Device(const Device &)			   = delete;
		Device & operator=(const Device &) = delete;

		Device(Device &&)			  = delete;
		Device & operator=(Device &&) = delete;

		void submit(const vk::CommandBuffer & cmd, vk::Fence fence) const;

		vk::Result present(const vk::PresentInfoKHR & presentInfo) const;

		// Helpers

		[[nodiscard]] u32 getMemoryType(u32 typeBits, vk::MemoryPropertyFlags properties, vk::Bool32 * memTypeFound = nullptr) const;

		// Getters

		[[nodiscard]] vk::Device getHandle() const { return m_handle.get(); }

		[[nodiscard]] const Gpu & getGpu() const { return m_gpu; }

		[[nodiscard]] u32 getQueueFamilyIndex() const { return m_gpu.graphicsQueueFamilyIndex; }

		[[nodiscard]] const std::vector<vk::ExtensionProperties> & getAvailableExtensions() const { return m_availableExtensions; }

		[[nodiscard]] const std::vector<const char *> & getEnabledExtensions() const { return m_enabledExtensions; }

		[[nodiscard]] vk::SurfaceKHR getSurface() const { return m_surface.get(); }

	private:
		static vk::UniqueSurfaceKHR createSurface(vk::Instance instance);

		vk::UniqueDevice m_handle;

		Gpu m_gpu;

		vk::UniqueSurfaceKHR m_surface{nullptr};

		std::vector<vk::ExtensionProperties> m_availableExtensions;

		std::vector<const char *> m_enabledExtensions;

		vk::Queue m_graphicsQueue;

		mutable std::mutex m_queueMutex{};

		Logger m_logger{"vulkan"};
	};

} // namespace gen