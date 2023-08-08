// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <limits>
#include <map>
#include <memory> // std::unique_ptr
#include <set>

#include <vulkan/vulkan.hpp>
#ifndef GLFW_INCLUDE_NONE
	#  define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

#include "core.hpp"
#include "logger/log.hpp"
#include "windowing/window.hpp"

namespace vk::util
{

	std::vector<char const *> gatherExtensions(std::vector<std::string> const & extensions
#ifndef GEN_NDEBUG
											   ,
											   std::vector<vk::ExtensionProperties> const & extensionProperties
#endif
	);

	vk::StructureChain<vk::InstanceCreateInfo> makeInstanceCreateInfoChain(vk::ApplicationInfo const & appInfo, std::vector<const char *> const & layers, std::vector<const char *> const & extensions);

	vk::DebugUtilsMessengerCreateInfoEXT makeDebugUtilsMessengerCreateInfoEXT();

	bool checkDeviceExtensionSupport(vk::PhysicalDevice device, const std::vector<const char *> & deviceExtensions);

	vk::UniqueShaderModule createShaderModule(vk::Device const & device, vk::ShaderStageFlagBits shaderStage, std::string const & shaderText);

	vk::UniqueSurfaceKHR createWindowSurface(vk::Instance const & instance, gen::Window const & window);

} // namespace vk::util
