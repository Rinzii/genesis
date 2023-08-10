// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/device.hpp"
#include "graphics/vkHelpers.hpp"
#include "logger/log.hpp"

#ifndef GLFW_INCLUDE_NONE
	#  define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

#include <format>
#include <numeric>
#include <string>

namespace gen
{

	GraphicsDevice::GraphicsDevice(Window &window, std::string const & appName)
	{
		// TODO: Allow for the vulkan api to be specified in a config file or through command line arguments.
		// We could also dynamically check for the highest supported version of the vulkan api, but that feels outside the scope of this project.
		createInstance(appName, "Genesis Engine", VK_API_VERSION_1_3);
		createSurface(window);
		createDevice();
	}

	void GraphicsDevice::createInstance(const std::string & appName, const std::string & engineName, const gen::u32 & apiVersion)
	{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader const dynLoader;
		auto vkGetInstanceProcAddr = dynLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif

		// TODO: Allow for the application to be able to specify its version itself. Instead of just using the engine version.
		vk::ApplicationInfo const appInfo(appName.c_str(), gen::version_v.getVersion(), engineName.c_str(), gen::version_v.getVersion(), apiVersion);

		auto extensionsCount	   = 0U;
		auto * requestedExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);
		std::vector<std::string> const requestedExtensionsVec(requestedExtensions,
														requestedExtensions + extensionsCount); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		auto enabledExtensions = vk::util::gatherExtensions(requestedExtensionsVec
#ifndef GEN_NDEBUG
															,
															vk::enumerateInstanceExtensionProperties()
#endif
		);

		gen::logger::debug("vulkan",
						   std::format("Enabled extensions: \n\t{}", std::accumulate(enabledExtensions.begin(), enabledExtensions.end(), std::string(),
																					 [](const std::string & acc, const std::string & ext)
																					 { return acc.empty() ? ext : acc + ", \n\t" + ext; })));

		m_instance = vk::createInstanceUnique(vk::util::makeInstanceCreateInfoChain(appInfo, {}, enabledExtensions).get<vk::InstanceCreateInfo>());

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_instance);
#endif
	}
	void GraphicsDevice::createSurface(Window &window)
	{
		m_surface = vk::util::createWindowSurface(m_instance.get(), window);
		gen::logger::debug("vulkan", "Created surface");
	}

	void GraphicsDevice::createDevice()
	{
		vk::PhysicalDevice const physicalDevice = m_instance->enumeratePhysicalDevices().front();

		u32 propertyCount{};
		physicalDevice.getQueueFamilyProperties(&propertyCount, nullptr);

		// get the QueueFamilyProperties of the first PhysicalDevice
		std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

		// get the first index into queueFamiliyProperties which supports graphics
		std::size_t const graphicsQueueFamilyIndex =
			std::distance( queueFamilyProperties.begin(),
						  std::find_if( queueFamilyProperties.begin(),
									   queueFamilyProperties.end(),
									   []( vk::QueueFamilyProperties const & qfp )
									   {
										   return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
									   }));
		assert( graphicsQueueFamilyIndex < queueFamilyProperties.size() );

		gen::logger::debug("vulkan", std::format("Found {} queue families", queueFamilyProperties.size()));
        for (std::size_t i = 0; i < queueFamilyProperties.size(); ++i)
        {
            gen::logger::debug("vulkan", std::format("Queue family {} supports {} queues", i, queueFamilyProperties[i].queueCount));
        }


		// now create our unique device
		float const queuePriority = 0.0F;
		vk::DeviceQueueCreateInfo deviceQueueCreateInfo( vk::DeviceQueueCreateFlags(), static_cast<uint32_t>( graphicsQueueFamilyIndex ), 1, &queuePriority );

		m_device = physicalDevice.createDeviceUnique( vk::DeviceCreateInfo( vk::DeviceCreateFlags(), deviceQueueCreateInfo ) );

	}



} // namespace gen
