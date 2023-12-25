// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/vk/devices/logicalDevice.hpp"
#include "gen/logger/log.hpp"
#include "gen/rendering/utils/debugUtils.hpp"
#include "gen/rendering/utils/exceptions.hpp"
#include "gen/rendering/utils/vkHelpers.hpp"
#include "gen/system/types.hpp"

#include "gen/rendering/renderWindow.hpp"
#include "gen/rendering/vk/devices/instance.hpp"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

namespace gen
{
	namespace
	{
		const Logger s_logger{"vulkan"};
		constexpr auto defaultQueuePriority_v = 1.0F;

		u32 findGraphicsQueueFamilyIndex(const vk::PhysicalDevice & pDevice, const vk::SurfaceKHR & surface)
		{
			u32 indices{};

			auto queueFamilies = pDevice.getQueueFamilyProperties();

			int index{0};
			for (const auto & queueFamily : queueFamilies)
			{
				bool const hasGraphicsFlag = (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) != vk::QueueFlagBits(0);
				bool const hasTransferFlag = (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer) != vk::QueueFlagBits(0);

				if (hasGraphicsFlag && hasTransferFlag &&
					pDevice.getSurfaceSupportKHR(static_cast<u32>(index), surface)) // NOLINT(readability-implicit-bool-conversion)
				{
					indices = static_cast<u32>(index);
					break;
				}

				index++;
			}

			return indices;
		}

	} // namespace

	Device::Device(
		Instance & instance,
		PhysicalDevice & gpu,
		std::span<const char * const> requiredExtensions,
		const vk::PhysicalDeviceFeatures & requiredFeatures,
		const vk::PhysicalDeviceFeatures & optionalFeatures,
		std::optional<u32> selectSpecificGpuIndex)
		: m_gpu(gpu)
	{
		auto const & pDevice = m_gpu.getHandle();

		m_surface = RenderWindow::self().createSurface(instance.getHandle());

		auto const graphicsQueueFamilyIndex = findGraphicsQueueFamilyIndex(pDevice, m_surface);
	}

} // namespace gen