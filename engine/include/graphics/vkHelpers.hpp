// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"
#include <vulkan/vulkan.hpp>
#include <vector>
#include <string>

namespace vk::util {

VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessengerCallback( VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
														   VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
														   VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
														   void * /*pUserData*/ )
{
#if !defined(NDEBUG)
	if (pCallbackData->messageIdNumber == 648835635) {
		// UNASSIGNED-khronos-Validation-debug-build-warning-message
		return VK_FALSE;
	}
	if (pCallbackData->messageIdNumber == 767975156) {
		// UNASSIGNED-BestPractices-vkCreateInstance-specialuse-extension
		return VK_FALSE;
	}
#endif
}

    std::vector<char const*> gatherExtensions(std::vector<std::string> const& extensions
    #if !defined(NDEBUG) || !defined(GEN_NDEBUG)
                                              ,
                                              std::vector<vk::ExtensionProperties> const& extensionProperties
    #endif
    ) {
        std::vector<char const*> enabledExtensions;
        enabledExtensions.reserve(extensions.size());
        for (auto const& ext : extensions) {
            assert(
                std::any_of(extensionProperties.begin(), extensionProperties.end(), [ext](vk::ExtensionProperties const& ep) { return ext == ep.extensionName; }));
            enabledExtensions.push_back(ext.data());
        }
    #if !defined(NDEBUG) || !defined(GEN_NDEBUG)
        if (std::none_of(extensions.begin(), extensions.end(), [](std::string const& extension) { return extension == VK_EXT_DEBUG_UTILS_EXTENSION_NAME; }) &&
            std::any_of(extensionProperties.begin(), extensionProperties.end(),
                        [](vk::ExtensionProperties const& ep) { return (strcmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, ep.extensionName) == 0); })) {
            enabledExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
    #endif
        return enabledExtensions;
    }

    std::vector<char const*> gatherLayers(std::vector<std::string> const& layers
    #if !defined(NDEBUG) || !defined(GEN_NDEBUG)
                                          ,
                                          std::vector<vk::LayerProperties> const& layerProperties
    #endif
    ) {
        std::vector<char const*> enabledLayers;
        enabledLayers.reserve(layers.size());
        for (auto const& layer : layers) {
            assert(std::any_of(layerProperties.begin(), layerProperties.end(), [layer](vk::LayerProperties const& lp) { return layer == lp.layerName; }));
            enabledLayers.push_back(layer.data());
        }
    #if !defined(NDEBUG) || !defined(GEN_NDEBUG)
        // Enable standard validation layer to find as much errors as possible!
        if (std::none_of(layers.begin(), layers.end(), [](std::string const& layer) { return layer == "VK_LAYER_KHRONOS_validation"; }) &&
            std::any_of(layerProperties.begin(), layerProperties.end(),
                        [](vk::LayerProperties const& lp) { return (strcmp("VK_LAYER_KHRONOS_validation", lp.layerName) == 0); })) {
            enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
        }
    #endif
        return enabledLayers;
    }

#if defined(NDEBUG) || defined(GEN_NDEBUG)
    vk::StructureChain<vk::InstanceCreateInfo>
#else
    vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT>
#endif
	makeInstanceCreateInfoChain( vk::ApplicationInfo const & appInfo,
                                 std::vector< const char * > const & layers,
                                 std::vector< const char * > const & extensions ) {

#if defined(NDEBUG) || defined(GEN_NDEBUG)
		// When in non-debug mode, use the InstanceCreateInfo for instance creation.
		vk::StructureChain<vk::InstanceCreateInfo> createInfoChain({}, &appInfo, layers, extensions);
#else
		// in debug mode, also use the debugUtilsMessengerCallback in instance creation
		vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
															vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
		vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
														   vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
		vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> instanceCreateInfo(
			{{}, &appInfo, layers, extensions}, {{}, severityFlags, messageTypeFlags, &vk::util::debugUtilsMessengerCallback});
#endif
		return instanceCreateInfo;
	}

	vk::DebugUtilsMessengerCreateInfoEXT makeDebugUtilsMessengerCreateInfoEXT()
	{
		return { {},
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
					vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
				&vk::util::debugUtilsMessengerCallback };
	}

} // namespace vk::util
