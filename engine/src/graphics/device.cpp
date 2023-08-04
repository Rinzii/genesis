// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/device.hpp"
#include "graphics/vkHelpers.hpp"
#include "logger/log.hpp"

#include <GLFW/glfw3.h>

namespace gen
{

	static const std::vector<std::string> validationLayers = {"VK_LAYER_KHRONOS_validation"};
	static const std::vector<std::string> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	GraphicsDevice::GraphicsDevice(gen::Window & window)
	{
		// TODO: Allow for the vulkan api to be specified in a config file or through command line arguments.
		// We could also dynamically check for the highest supported version of the vulkan api, but that feels outside the scope of this project.
		createInstance(window.getTitle(), "Genesis Engine", validationLayers, deviceExtensions, VK_API_VERSION_1_0);

	}

	GraphicsDevice::~GraphicsDevice()
	{

	}

	void GraphicsDevice::createInstance(const std::string & appName,
										const std::string & engineName,
										const std::vector<std::string> & layers,
										const std::vector<std::string> & extensions,
										const gen::u32 & apiVersion)
	{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader const vkdl;
		auto vkGetInstanceProcAddr = vkdl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif

		if (enableValidationLayers) //NOLINT validation layers are enabled conditionally and this trips up clang-tidy
        {
			if (!vk::util::checkValidationLayerSupport(validationLayers))
			{
				gen::logger::error("vulkan", "validation layers requested, but not available!");
				throw std::runtime_error("validation layers requested, but not available!");
			}
        }

		// TODO: Allow for the application to be able to specify its version itself. Instead of just using the engine version.
		vk::ApplicationInfo const appInfo(
			appName.c_str(),
			gen::version_v.getVersion(),
			engineName.c_str(),
			gen::version_v.getVersion(),
			apiVersion);

		vk::InstanceCreateInfo const createInfo({}, &appInfo);

		auto enabledLayers = vk::util::gatherLayers(layers
#ifndef GEN_NDEBUG
													,
													vk::enumerateInstanceLayerProperties()
#endif
		);

		auto enabledExtensions = vk::util::gatherExtensions(extensions
#ifndef GEN_NDEBUG
															,
															vk::enumerateInstanceExtensionProperties()
#endif
		);

		m_instance = vk::createInstanceUnique(vk::util::makeInstanceCreateInfoChain(appInfo, enabledLayers, enabledExtensions).get<vk::InstanceCreateInfo>());

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_instance);
#endif
	}

}
