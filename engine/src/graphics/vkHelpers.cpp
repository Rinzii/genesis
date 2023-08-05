// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/vkHelpers.hpp"
#include <numeric>
#include <vulkan/vulkan.hpp>
#if defined( VULKAN_HPP_NO_TO_STRING )
	#  include <vulkan/vulkan_to_string.hpp>
#endif
#include <spirv_cross.hpp>
#include <spirv_hlsl.hpp>
#include "logger/log.hpp"

#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
// NOLINTNEXTLINE
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

// Define the helper functions and classes within the 'vk::util' namespace.
namespace vk::util
{
	// Callback function used for Vulkan debug messaging.
	VkBool32 debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
										 VkDebugUtilsMessageTypeFlagsEXT messageTypes,
										 const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
										 void * /*pUserData*/)
	{
#ifndef GEN_NDEBUG
		// In debug mode, certain Vulkan validation layer messages are ignored.
		// Here, we filter out specific validation messages identified by their messageIdNumber.
		if ( pCallbackData->messageIdNumber == 648835635 ) //NOLINT UNASSIGNED-khronos-Validation-debug-build-warning-message
		{
			return vk::False;
		}
		if ( pCallbackData->messageIdNumber == 767975156 ) //NOLINT UNASSIGNED-BestPractices-vkCreateInstance-specialuse-extension
		{
			return vk::False;
		}
#endif

		// Log the Vulkan debug message.
		gen::logger::error("vulkan", std::format("{}: {}:\n "
												 "\tmessageIDName   = <{}>\n"
												 "\tmessageIDNumber = {}\n"
												 "\tmessage         = <{}>\n",
												 vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ) ),
												 vk::to_string( static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes ) ),
												 pCallbackData->pMessageIdName,
												 pCallbackData->messageIdNumber,
												 pCallbackData->pMessage));

		return vk::False;
	}

	// Gather enabled extensions based on the requested extensions and available extension properties.
	std::vector<char const*> gatherExtensions(std::vector<std::string> const& extensions
#ifndef GEN_NDEBUG
											   ,
											   std::vector<vk::ExtensionProperties> const& extensionProperties
#endif
	)
	{
		std::vector<char const *> enabledExtensions;
		enabledExtensions.reserve( extensions.size() );

		std::vector<std::string> availableExtensions;
		for ( auto const & eProp : extensionProperties )
		{
			availableExtensions.push_back( eProp.extensionName );
		}

		gen::logger::debug("vulkan", std::format("Available extensions: \n\t{}",
												 std::accumulate(availableExtensions.begin(),
																 availableExtensions.end(),
																 std::string(),
																 [](const std::string &acc, const std::string &ext)
																 {
																	 return acc.empty() ? ext : acc + ", \n\t" + ext;
																 })));

		for ( auto const & ext : extensions )
		{
			gen::logger::info("vulkan", std::format("Enabling extension: {}", ext));


			assert(std::any_of(extensionProperties.begin(),
							   extensionProperties.end(),
							   [ext]( vk::ExtensionProperties const & eProp )
							   {
								   return ext == eProp.extensionName;
							   })
				   );

			enabledExtensions.push_back( ext.data() );
		}

#ifndef GEN_NDEBUG
		if (std::none_of(extensions.begin(),
						 extensions.end(),
						 [](std::string const & extension)
						 {
							 return extension == VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
						 }) &&
			std::any_of(extensionProperties.begin(),
						   extensionProperties.end(),
						   []( vk::ExtensionProperties const & eProp )
						   {
							   return ( std::strcmp( VK_EXT_DEBUG_UTILS_EXTENSION_NAME, eProp.extensionName ) == 0 );
						   }))
		{
			enabledExtensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
		}
#endif
		return enabledExtensions;
	}

	// Gather enabled layers based on the requested layers and available layer properties.
	std::vector<char const*> gatherLayers(std::vector<std::string> const& layers
#ifndef GEN_NDEBUG
										   ,
										   std::vector<vk::LayerProperties> const& layerProperties
#endif
	)
	{
		std::vector<char const*> enabledLayers;
		enabledLayers.reserve(layers.size());
		for (auto const& layer : layers)
		{
			// Check if the requested layer exists in the available layer properties.
			assert(std::any_of(layerProperties.begin(),
							   layerProperties.end(),
							   [layer](vk::LayerProperties const& lProp)
							   {
								   return layer == lProp.layerName;
							   })
				   );

			enabledLayers.push_back(layer.data());
		}
#ifndef GEN_NDEBUG
		// In debug mode, enable the "VK_LAYER_KHRONOS_validation" standard validation layer if not already enabled.
		if (std::none_of(layers.begin(),
						 layers.end(),
						 [](std::string const& layer)
						 {
							 return layer == "VK_LAYER_KHRONOS_validation";
						 })
			&& std::any_of(layerProperties.begin(),
						   layerProperties.end(),
						   [](vk::LayerProperties const& lProp)
						   {
							   return (std::strcmp("VK_LAYER_KHRONOS_validation", lProp.layerName) == 0);
						   }))
		{
			enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
		}
#endif
		return enabledLayers;
	}

	// Create an instance create info chain with or without debug utils messenger in debug mode.
#ifdef GEN_NDEBUG
	vk::StructureChain<vk::InstanceCreateInfo>
#else
	vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT>
#endif
	makeInstanceCreateInfoChain(vk::ApplicationInfo const & appInfo,
								std::vector< const char * > const & layers,
								std::vector< const char * > const & extensions)
	{

#ifdef GEN_NDEBUG
		// When in non-debug mode, use the InstanceCreateInfo for instance creation.
		vk::StructureChain<vk::InstanceCreateInfo> const instanceCreateInfo({{}, &appInfo, layers, extensions});
#else
		// In debug mode, also use the debugUtilsMessengerCallback in instance creation
		vk::DebugUtilsMessageSeverityFlagsEXT const severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
																  vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);

		vk::DebugUtilsMessageTypeFlagsEXT const messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
																 vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
																 vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

		vk::StructureChain<vk::InstanceCreateInfo,
						   vk::DebugUtilsMessengerCreateInfoEXT> instanceCreateInfo(
				{{}, &appInfo, layers, extensions},
				{{}, severityFlags, messageTypeFlags, &vk::util::debugUtilsMessengerCallback});
#endif
		return instanceCreateInfo;
	}

	// Create DebugUtilsMessengerCreateInfoEXT for Vulkan debug messaging.
	vk::DebugUtilsMessengerCreateInfoEXT makeDebugUtilsMessengerCreateInfoEXT()
	{
		return { {},
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
					vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
				&vk::util::debugUtilsMessengerCallback };
	}

	// Check if the requested validation layers are supported by the Vulkan instance.
	bool checkValidationLayerSupport(const std::vector<std::string>& validationLayers)
	{
#ifndef GEN_NDEBUG
		auto availableLayers = vk::enumerateInstanceLayerProperties();

		for (auto const& layerName : validationLayers)
		{
			bool layerFound = false;

			for (auto const& layerProperties : availableLayers)
			{
				if (layerName == layerProperties.layerName)
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

	// Check if the required device extensions are supported by the given physical device.
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

	// Convert HLSL shader code to SPIR-V format using SPIRV-Cross.
	bool HLSLtoSPV(const vk::ShaderStageFlagBits shaderType, std::string const& hlslShader, std::vector<unsigned int>& spvShader)
	{
		// Map Vulkan shader stage to SPIRV-Cross shader stage
		spv::ExecutionModel executionModel {};
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

	// Create a Vulkan shader module from HLSL shader code.
	vk::UniqueShaderModule createShaderModule( vk::Device const & device, vk::ShaderStageFlagBits shaderStage, std::string const & shaderText )
	{
		std::vector<unsigned int> shaderSPV;
		if (!HLSLtoSPV(shaderStage, shaderText, shaderSPV))
		{ // TODO: Decide if we want to not throw an exception if compilation fails
			gen::logger::error("vulkan", "Failed to compile shader");
			throw std::runtime_error("Failed to compile shader");
		}

		return device.createShaderModuleUnique(vk::ShaderModuleCreateInfo( vk::ShaderModuleCreateFlags(), shaderSPV ) );
	}

	vk::UniqueSurfaceKHR createWindowSurface( vk::UniqueInstance const & instance, gen::Window const & window )
	{
		VkSurfaceKHR surface_ {};
		VkResult const err = glfwCreateWindowSurface( VkInstance( instance.get() ), window.getHandle(), nullptr, &surface_ );
		if ( err != VK_SUCCESS )
		{
			gen::logger::error( "vulkan", "Failed to create window surface!" );
			throw std::runtime_error( "Failed to create window!" );
		}
		vk::ObjectDestroy<vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE> const deleter_( instance.get() );
		return vk::UniqueSurfaceKHR( vk::SurfaceKHR( surface_ ), deleter_ );
	}

} // namespace vk::util
