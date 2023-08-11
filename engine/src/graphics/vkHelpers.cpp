// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/vkHelpers.hpp"

#include <format>
#include <numeric>
#include <span>
#include <unordered_set>

#if defined(VULKAN_HPP_NO_TO_STRING)
	#include <vulkan/vulkan_to_string.hpp>
#endif
#include <spirv_cross.hpp>
#include <spirv_hlsl.hpp>
#include "logger/log.hpp"

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
// NOLINTNEXTLINE
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

// Define the helper functions and classes within the 'vk::util' namespace.
namespace vk::util
{


	// Gather enabled extensions based on the requested extensions and available extension properties.
	std::vector<char const *> gatherExtensions(std::vector<std::string> const & extensions
#ifndef GEN_NDEBUG
											   ,
											   std::vector<vk::ExtensionProperties> const & extensionProperties
#endif
	)
	{
		std::vector<char const *> enabledExtensions{};
		enabledExtensions.reserve(extensions.size());

		std::vector<std::string> availableExtensions{};

#ifndef GEN_NDEBUG
		for (auto const & eProp : extensionProperties) { availableExtensions.push_back(eProp.extensionName); }
#endif

		gen::logger::debug("vulkan",
						   std::format("Available extensions: \n\t{}", std::accumulate(availableExtensions.begin(), availableExtensions.end(), std::string(),
																					   [](const std::string & acc, const std::string & ext)
																					   { return acc.empty() ? ext : acc + ", \n\t" + ext; })));

		for (auto const & ext : extensions)
		{
			assert(std::any_of(extensionProperties.begin(), extensionProperties.end(),
							   [ext](vk::ExtensionProperties const & eProp) { return ext == eProp.extensionName; }));

			enabledExtensions.push_back(ext.data());
		}

		return enabledExtensions;
	}

	// Create an instance create info chain with or without debug utils messenger in debug mode.
	vk::StructureChain<vk::InstanceCreateInfo> makeInstanceCreateInfoChain(const vk::ApplicationInfo & appInfo, std::vector<const char *> const & layers, std::vector<const char *> const & extensions)
	{

		// When in non-debug mode, use the InstanceCreateInfo for instance creation.
		vk::StructureChain<vk::InstanceCreateInfo> const instanceCreateInfo({{}, &appInfo, layers, extensions});

		return instanceCreateInfo;
	}


	// Check if the required device extensions are supported by the given physical device.
	bool checkDeviceExtensionSupport(const vk::PhysicalDevice device, const std::span<const char * const> deviceExtensions)
	{
		auto availableExtensions = device.enumerateDeviceExtensionProperties();

		std::unordered_set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto & extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
			if (requiredExtensions.empty())
			{
				return true; // All required extensions are supported, early exit.
			}
		}

		return false; // Some required extensions are not supported.
	}

	// Convert HLSL shader code to SPIR-V format using SPIRV-Cross.
	bool HLSLtoSPV(const vk::ShaderStageFlagBits shaderType, std::string const & hlslShader, std::vector<unsigned int> & spvShader)
	{
		// Map Vulkan shader stage to SPIRV-Cross shader stage
		spv::ExecutionModel executionModel; // NOLINT(cppcoreguidelines-init-variables)
		switch (shaderType)
		{
		case vk::ShaderStageFlagBits::eVertex: executionModel = spv::ExecutionModelVertex; break;
		case vk::ShaderStageFlagBits::eFragment: executionModel = spv::ExecutionModelFragment; break;
		case vk::ShaderStageFlagBits::eCompute: executionModel = spv::ExecutionModelGLCompute; break;
		case vk::ShaderStageFlagBits::eGeometry: executionModel = spv::ExecutionModelGeometry; break;
		// TODO: Add more shader stages as needed
		default: gen::logger::warn("vulkan", "HLSLtoSPV: Unsupported shader type"); return false;
		}

		// Compile HLSL to SPIR-V using SPIRV-Cross
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
		spirv_cross::CompilerHLSL compiler(reinterpret_cast<const uint32_t *>(hlslShader.data()),
										   hlslShader.size() / sizeof(uint32_t));
		compiler.set_entry_point("main", executionModel);

		std::basic_string<char, std::char_traits<char>, std::allocator<char>> spirvData = compiler.compile();
		spvShader																		= std::vector<unsigned int>(spirvData.begin(), spirvData.end());

		return true;
	}

	// Create a Vulkan shader module from HLSL shader code.
	vk::UniqueShaderModule createShaderModule(const vk::Device device, vk::ShaderStageFlagBits shaderStage, std::string const & shaderText)
	{
		std::vector<unsigned int> shaderSPV;
		if (!HLSLtoSPV(shaderStage, shaderText, shaderSPV))
		{ // TODO: Decide if we want to not throw an exception if compilation fails
			gen::logger::error("vulkan", "Failed to compile shader");
			throw std::runtime_error("Failed to compile shader");
		}

		return device.createShaderModuleUnique(vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), shaderSPV));
	}

	vk::UniqueSurfaceKHR createWindowSurface(const vk::Instance instance, gen::Window const & window)
	{
		VkSurfaceKHR surface_{};
		VkResult const err = glfwCreateWindowSurface(VkInstance(instance), window.getHandle(), nullptr, &surface_);
		if (err != VK_SUCCESS)
		{
			gen::logger::error("vulkan", "Failed to create window surface!");
			throw std::runtime_error("Failed to create window surface!");
		}

		return vk::UniqueSurfaceKHR(surface_, instance);
	}

} // namespace vk::util
