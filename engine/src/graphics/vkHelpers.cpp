// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/vkHelpers.hpp"

#include <iomanip>
#include <numeric>


#include <vulkan/vulkan.hpp>
#if defined( VULKAN_HPP_NO_TO_STRING )
	#  include <vulkan/vulkan_to_string.hpp>
#endif


#include <spirv_cross.hpp>
#include <spirv_hlsl.hpp>

#include "logger/log.hpp"

#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif


namespace vk::util
{

	VkBool32 debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
										 VkDebugUtilsMessageTypeFlagsEXT messageTypes,
										 const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
										 void *)
	{
#if !defined( NDEBUG ) || !defined( GEN_NDEBUG )
		if ( pCallbackData->messageIdNumber == 648835635 )
		{
			// UNASSIGNED-khronos-Validation-debug-build-warning-message
			return VK_FALSE;
		}
		if ( pCallbackData->messageIdNumber == 767975156 )
		{
			// UNASSIGNED-BestPractices-vkCreateInstance-specialuse-extension
			return VK_FALSE;
		}
#endif

		gen::logger::error("vulkan", std::format("{}: {}:\n "
												 "\tmessageIDName   = <{}>\n"
												 "\tmessageIDNumber = {}\n"
												 "\tmessage         = <{}>\n",
												 vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ) ),
												 vk::to_string( static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes ) ),
												 pCallbackData->pMessageIdName,
												 pCallbackData->messageIdNumber,
												 pCallbackData->pMessage));

		return VK_FALSE;
	}

	std::vector<char const*> gatherExtensions(std::vector<std::string> const& extensions
#if !defined(NDEBUG) || !defined(GEN_NDEBUG)
											   ,
											   std::vector<vk::ExtensionProperties> const& extensionProperties
#endif
	)
	{
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
	)
	{
		std::vector<char const*> enabledLayers;
		enabledLayers.reserve(layers.size());
		for (auto const& layer : layers)
		{
			assert(std::any_of(layerProperties.begin(), layerProperties.end(), [layer](vk::LayerProperties const& lp) { return layer == lp.layerName; }));
			enabledLayers.push_back(layer.data());
		}
#if !defined(NDEBUG) || !defined(GEN_NDEBUG)
		// Enable standard validation layer to find as much errors as possible!
		if (std::none_of(layers.begin(), layers.end(), [](std::string const& layer) { return layer == "VK_LAYER_KHRONOS_validation"; }) &&
			std::any_of(layerProperties.begin(), layerProperties.end(),
						[](vk::LayerProperties const& lp) { return (strcmp("VK_LAYER_KHRONOS_validation", lp.layerName) == 0); }))
		{
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
								std::vector< const char * > const & extensions )
	{

#if defined(NDEBUG) || defined(GEN_NDEBUG)
		// When in non-debug mode, use the InstanceCreateInfo for instance creation.
		vk::StructureChain<vk::InstanceCreateInfo> createInfoChain({}, &appInfo, layers, extensions);
#else
		// in debug mode, also use the debugUtilsMessengerCallback in instance creation
		vk::DebugUtilsMessageSeverityFlagsEXT const severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
																  vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
		vk::DebugUtilsMessageTypeFlagsEXT const messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
																 vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
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

	bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers)
	{
#if !defined(NDEBUG) || !defined(GEN_NDEBUG)
		auto availableLayers = vk::enumerateInstanceLayerProperties();

		for (auto const& layerName : validationLayers)
		{
			bool layerFound = false;

			for (auto const& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
#else
		return true;
#endif
	}

	bool checkDeviceExtensionSupport(vk::PhysicalDevice device, const std::vector<const char*>& deviceExtensions)
	{
		auto availableExtensions = device.enumerateDeviceExtensionProperties();

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (auto const& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	bool HLSLtoSPV(const vk::ShaderStageFlagBits shaderType, std::string const& hlslShader, std::vector<unsigned int>& spvShader)
	{
		// Map Vulkan shader stage to SPIRV-Cross shader stage
		spv::ExecutionModel executionModel;
		switch (shaderType)
		{
		case vk::ShaderStageFlagBits::eVertex:
			executionModel = spv::ExecutionModelVertex;
			break;
		case vk::ShaderStageFlagBits::eFragment:
			executionModel = spv::ExecutionModelFragment;
			break;
		case vk::ShaderStageFlagBits::eCompute:
			executionModel = spv::ExecutionModelGLCompute;
            break;
		case vk::ShaderStageFlagBits::eGeometry:
            executionModel = spv::ExecutionModelGeometry;
            break;
		// TODO: Add more shader stages as needed
		default:
			gen::logger::warn("vulkan", "HLSLtoSPV: Unsupported shader type");
			return false;
		}

		// Compile HLSL to SPIR-V using SPIRV-Cross
		spirv_cross::CompilerHLSL compiler(reinterpret_cast<const uint32_t*>(hlslShader.data()), hlslShader.size() / sizeof(uint32_t)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
		compiler.set_entry_point("main", executionModel);

		std::basic_string<char, std::char_traits<char>, std::allocator<char>> spirvData = compiler.compile();
		spvShader = std::vector<unsigned int>(spirvData.begin(), spirvData.end());

		return true;
	}

	vk::ShaderModule createShaderModule( vk::Device const & device, vk::ShaderStageFlagBits shaderStage, std::string const & shaderText )
    {
        std::vector<unsigned int> shaderSPV;
        if (!HLSLtoSPV(shaderStage, shaderText, shaderSPV))
        { // TODO: Decide if we want to not throw an exception if compilation fails
            gen::logger::error("vulkan", "Failed to compile shader");
            throw std::runtime_error("Failed to compile shader");
        }

		return device.createShaderModule( vk::ShaderModuleCreateInfo( vk::ShaderModuleCreateFlags(), shaderSPV ) );
    }

}
