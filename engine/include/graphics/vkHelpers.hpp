// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <limits>
#include <map>
#include <memory> // std::unique_ptr
#include <set>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "core.hpp"
#include "logger/log.hpp"
#include "windowing/window.hpp"

namespace vk::util
{

	VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
															   VkDebugUtilsMessageTypeFlagsEXT messageTypes,
															   VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData, void * /*pUserData*/);

	std::vector<char const *> gatherExtensions(std::vector<std::string> const & extensions
#ifndef GEN_NDEBUG
											   ,
											   std::vector<vk::ExtensionProperties> const & extensionProperties
#endif
	);

	std::vector<char const *> gatherLayers(std::vector<std::string> const & layers
#ifndef GEN_NDEBUG
										   ,
										   std::vector<vk::LayerProperties> const & layerProperties
#endif
	);

#ifdef GEN_NDEBUG
	vk::StructureChain<vk::InstanceCreateInfo>
#else
	vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT>
#endif
	makeInstanceCreateInfoChain(vk::ApplicationInfo const & appInfo, std::vector<const char *> const & layers, std::vector<const char *> const & extensions);

	vk::DebugUtilsMessengerCreateInfoEXT makeDebugUtilsMessengerCreateInfoEXT();

	bool checkValidationLayerSupport(const std::vector<std::string> & validationLayers);

	bool checkDeviceExtensionSupport(vk::PhysicalDevice device, const std::vector<const char *> & deviceExtensions);

	vk::UniqueShaderModule createShaderModule(vk::Device const & device, vk::ShaderStageFlagBits shaderStage, std::string const & shaderText);

	vk::UniqueSurfaceKHR createWindowSurface(vk::Instance const & instance, gen::Window const & window);

} // namespace vk::util
