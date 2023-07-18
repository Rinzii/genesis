// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/device.hpp"
#include "graphics/vkHelpers.hpp"

namespace gen
{

	GraphicsDevice::GraphicsDevice(gen::Window & window) : m_window{window}
	{
		createInstance(window.GetTitle(), "Genesis Engine", {}, {}, VK_API_VERSION_1_0);
	    createDebugMessenger();
	}

	GraphicsDevice::~GraphicsDevice()
	{
		m_device.destroyCommandPool(m_commandPool);
        m_device.destroy();

#if !defined( NDEBUG ) || !defined( GEN_NDEBUG )
		m_instance.destroyDebugUtilsMessengerEXT( m_debugMessenger );
#endif

		m_instance.destroySurfaceKHR(m_surface);
		m_instance.destroy();
	}

	void GraphicsDevice::createInstance(const std::string & appName, const std::string & engineName, const std::vector<std::string> & layers,
											 const std::vector<std::string> & extensions, const gen::u32 & apiVersion)
	{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader const vkdl;
		auto vkGetInstanceProcAddr = vkdl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif
		if (enableValidationLayers && !vk::util::checkValidationLayerSupport(validationLayers))
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }

		vk::ApplicationInfo const appInfo(
			appName.c_str(),
			gen::version_v.getVersion(),
			engineName.c_str(),
			gen::version_v.getVersion(),
			apiVersion);

		vk::InstanceCreateInfo const createInfo({}, &appInfo);

		auto enabledLayers = vk::util::gatherLayers(layers
#if !defined(NDEBUG) || !defined(GEN_NDEBUG)
													,
													vk::enumerateInstanceLayerProperties()
#endif
		);

		auto enabledExtensions = vk::util::gatherExtensions(extensions
#if !defined(NDEBUG) || !defined(GEN_NDEBUG)
															,
															vk::enumerateInstanceExtensionProperties()
#endif
		);

		m_instance = vk::createInstance(vk::util::makeInstanceCreateInfoChain(appInfo, enabledLayers, enabledExtensions).get<vk::InstanceCreateInfo>());

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(m_instance);
#endif
	}


	void GraphicsDevice::createDebugMessenger()
	{
#if !defined(NDEBUG) || !defined(GEN_NDEBUG)
		m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(vk::util::makeDebugUtilsMessengerCreateInfoEXT());
#endif
	}
}
