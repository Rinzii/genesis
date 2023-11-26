// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/vk/devices/instance.hpp"

#include <GLFW/glfw3.h>

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
// NOLINTNEXTLINE
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif


namespace gen
{
	namespace
	{
		const Logger s_logger{"vulkan"};
	}

#if defined(GEN_DEBUG) || defined(GEN_FORCE_VALIDATION_LAYERS)
	VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes,
												 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)

		else if (messageSeverity &  VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
			Log::Info(pCallbackData->pMessage, '\n');
		else // VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
			Log::Error(pCallbackData->pMessage, '\n');

		return VK_FALSE;
	}

#endif


	Instance::Instance(const std::string & appName, gen::u32 appVersion, const std::string & engineName, gen::u32 engineVersion, gen::u32 apiVersion)
	{
		assert(apiVersion >= s_minimumApiVersion && "Vulkan API version is too low");

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader const dynLoader;
		auto vkGetInstanceProcAddr = dynLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif

		vk::ApplicationInfo const appInfo(appName.c_str(), appVersion, engineName.c_str(), engineVersion, apiVersion);

		// Get required extensions by glfw
		auto extensionsCount		  = 0U;
		auto * requiredGlfwExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);

		std::vector<std::string> const requiredGlfwExtensionsVec(
			requiredGlfwExtensions, requiredGlfwExtensions + extensionsCount); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

		auto enabledExtensions = vk::utils::gatherExtensions(
			requiredGlfwExtensionsVec
	}

}