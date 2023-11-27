// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/vk/devices/instance.hpp"
#include "gen/rendering/utils/exceptions.hpp"
#include "gen/core/base/config/platform.hpp"
#include "gen/logger/log.hpp"

#include <GLFW/glfw3.h>
#include <volk.h>


// Macros used for configuring Vulkan
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
// NOLINTNEXTLINE
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

#if (defined(GEN_DEBUG) || defined(GEN_VK_FORCE_VALIDATION_LAYERS)) && !defined(GEN_VK_DISABLE_VALIDATION_LAYERS) && !(defined(GEN_NDEBUG) && !defined(GEN_VK_FORCE_VALIDATION_LAYERS))
#define INTERNAL_USE_VALIDATION_LAYERS
#endif

#if defined(GEN_PLATFORM_APPLE)
#define INTERNAL_ENABLE_PORTABILITY
#endif

namespace gen
{
	namespace
	{
		const Logger s_logger{"vulkan"};

	#if defined(INTERNAL_USE_VALIDATION_LAYERS)
		VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT  /*messageTypes*/,
													 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void * /*pUserData*/)
		{
			if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) != 0)
			{
				GEN_LOG_WARN(s_logger, "{} - {}: {}", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage);
			}
			else if ((messageSeverity &  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) != 0)
			{
				GEN_LOG_ERROR(s_logger, "{} - {}: {}", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage);
			}

			return VK_FALSE;
		}
	#endif

		bool enableExtension(const char *extensionName, const std::vector<vk::ExtensionProperties> &availableExtensions, std::vector<const char *> &enabledExtensions)
		{
			for (auto const &availableExtension : availableExtensions)
			{
				if (std::strcmp(extensionName, availableExtension.extensionName) == 0)
				{
					auto iter = std::find_if(enabledExtensions.begin(),
											 enabledExtensions.end(),
											 [extensionName](const char *enabledExtension)
											 {
												 return std::strcmp(extensionName, enabledExtension) == 0;
											 });

					if (iter != enabledExtensions.end())
					{
						// Already enabled
					}
					else
					{
						GEN_LOG_INFO(s_logger, "Enabling extension: {}", extensionName);
						enabledExtensions.push_back(extensionName);
					}

					return true;
				}
			}

			GEN_LOG_INFO(s_logger, "Extension {} not found", extensionName);
			return false;
		}

		bool enableLayer(const char *layerName, const std::vector<vk::LayerProperties> &availableLayers, std::vector<const char *> &enabledLayers)
		{
#ifdef INTERNAL_USE_VALIDATION_LAYERS // Don't bother trying to enable layers if they are disabled
			for (auto const &availableLayer : availableLayers)
			{
				if (std::strcmp(layerName, availableLayer.layerName) == 0)
				{
					auto iter = std::find_if(enabledLayers.begin(),
											 enabledLayers.end(),
											 [layerName](const char *enabledLayer)
											 {
												 return std::strcmp(layerName, enabledLayer) == 0;
											 });

					if (iter != enabledLayers.end())
					{
						// Already enabled
					}
					else
					{
						GEN_LOG_INFO(s_logger, "Enabling layer: {}", layerName);
						enabledLayers.push_back(layerName);
					}

					return true;
				}
			}
			GEN_LOG_INFO(s_logger, "Layer {} not found", layerName);
#else
			GEN_LOG_WARN(s_logger, "Validation layers are disabled, cannot enable layer {}", layerName);
#endif

			return false;
		}

	} // namespace

	Instance::Instance(
		const std::string & appName,
		u32 appVersion,
		const std::string & engineName,
		u32 engineVersion,
		bool enableValidationLayers,
		const std::vector<std::string> & requiredExtensions,
		const std::vector<std::string> & requiredLayers,
		u32 apiVersion)
	{
		assert(!appName.empty() && "Application name cannot be empty");
		assert(!engineName.empty() && "Engine name cannot be empty");

#ifndef INTERNAL_USE_VALIDATION_LAYERS
		// If validation layers are disabled, we cannot enable validation layers
		if (enableValidationLayers)
		{
			GEN_LOG_WARN(s_logger, "Validation layers are disabled, cannot enable validation layers");
		}
		enableValidationLayers = false;
#endif

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader const dynLoader;
		auto vkGetInstanceProcAddr = dynLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif

		// Check if the selected Vulkan API version meets the minimum requirements
		doesProvidedApiMeetMin(apiVersion);

		const auto appInfo = vk::ApplicationInfo(appName.c_str(), appVersion, engineName.c_str(), engineVersion, apiVersion);


		// Handle the extensions

		const auto availableExtensions = vk::enumerateInstanceExtensionProperties();

		std::vector<const char *> desiredInstanceExtensions = {
#if defined(INTERNAL_USE_VALIDATION_LAYERS)
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
			VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME,
#endif
#if defined(GEN_PLATFORM_APPLE) // Required for MoltenVK
			VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
			VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
#endif
		};

		// Add the required extensions to the desired extensions vector
		for (auto const & ext : requiredExtensions)
		{
			enableExtension(ext.c_str(), availableExtensions, desiredInstanceExtensions);
		}

		// Since this extension loads some dynamic libraries it could take a few seconds to load.
		std::uint32_t glfwExtensionCount = 0;
		auto *requiredGlfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);


		// Add the required glfw extensions to the desired extensions vector
		for (std::uint32_t i = 0; i < glfwExtensionCount; i++)
		{
			enableExtension(requiredGlfwExtensions[i], availableExtensions, desiredInstanceExtensions); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		}

		// create a stream to output the enabled extensions to our logger
		auto enabledExtensionsStream = std::stringstream{};
		for (auto const & ext : desiredInstanceExtensions)
		{
			enabledExtensionsStream << ext << ", ";
		}
		GEN_LOG_INFO(s_logger, "Enabled extensions: {}", enabledExtensionsStream.str());


		// Handle the layers

		std::vector<const char *> desiredInstanceLayers = {};

		if (enableValidationLayers)
		{
			const auto supportedLayers = vk::enumerateInstanceLayerProperties();

			// Enable validation layers
			enableLayer("VK_LAYER_KHRONOS_validation", supportedLayers, desiredInstanceLayers);

			// Now add the requested required layers to the desired layers vector
			for (auto const & layer : requiredLayers) { enableLayer(layer.c_str(), supportedLayers, desiredInstanceLayers); }


			// create a stream to output the enabled extensions to our logger
			auto enabledLayersStream = std::stringstream{};
			for (auto const & layer : desiredInstanceLayers) { enabledLayersStream << layer << ", "; }
			GEN_LOG_INFO(s_logger, "Enabled layers: {}", enabledLayersStream.str());
		}


		// Handle instance creation

		auto instanceCreateInfo = vk::InstanceCreateInfo({}, &appInfo, desiredInstanceLayers, desiredInstanceExtensions);

#ifdef INTERNAL_ENABLE_PORTABILITY
		instanceCreateInfo.flags |= vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
#endif

		m_handle = vk::createInstanceUnique(instanceCreateInfo);

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_handle);
#endif

		// Volk is required to load all not yet implemented Vulkan-HPP functions
		volkLoadInstance(*m_handle);

		if (enableValidationLayers)
		{
			// Create the debug utils messenger
			debugUtilsMessengerExt = m_handle->createDebugUtilsMessengerEXTUnique(
				vk::DebugUtilsMessengerCreateInfoEXT({}, vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
													 vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
													 debugUtilsMessengerCallback));
		}

		// Query the physical devices and store them in the m_gpus vector
		queryPhysicalDevices();
	}

	Instance::Instance(vk::Instance instance)
	: m_handle(instance)
	{
		if (m_handle) { queryPhysicalDevices(); }
		else
		{
			throw VulkanException("Instance is not initialized");
		}
	}

	void Instance::queryPhysicalDevices()
	{
		assert(m_handle && "Instance is not initialized");

		auto physicalDevices = m_handle->enumeratePhysicalDevices();

		if (physicalDevices.empty())
		{
			throw VulkanException("No physical devices found");
		}

		for (auto const & physicalDevice : physicalDevices)
		{
			m_gpus.push_back(std::make_unique<PhysicalDevice>(physicalDevice));
		}

	}

	void Instance::doesProvidedApiMeetMin(u32 apiVersion)
	{
		// Check if the selected Vulkan API version meets the minimum requirements
		if (apiVersion < s_minimumApiVersion)
		{
			std::string const exceptionMessage = std::format(
				"The selected Vulkan API version ({}) does not meet the minimum requirements. The minimum supported version is {}.{}.{}. Please update your graphics drivers!",
				std::to_string(VK_API_VERSION_MAJOR(apiVersion)),
				std::to_string(VK_API_VERSION_MAJOR(s_minimumApiVersion)),
				std::to_string(VK_API_VERSION_MINOR(s_minimumApiVersion)),
				std::to_string(VK_API_VERSION_PATCH(s_minimumApiVersion)));

			throw VulkanException(exceptionMessage);
		}
	}


}