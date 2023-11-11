// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <map>
#include <memory> // std::unique_ptr
#include <set>

#include <vulkan/vulkan.hpp>
#ifndef GLFW_INCLUDE_NONE
	#define GLFW_INCLUDE_NONE
#endif
#include "GLFW/glfw3.h"

#include "gen/core.hpp"
#include "gen/logger/log.hpp"
#include "gen/windowing/window.hpp"

namespace vk::utils
{
	std::vector<char const *> gatherExtensions(
		std::vector<std::string> const & extensions
#ifndef GEN_NDEBUG
		,
		std::vector<vk::ExtensionProperties> const & extensionProperties
#endif
	);

	vk::StructureChain<vk::InstanceCreateInfo> makeInstanceCreateInfoChain(
		const vk::ApplicationInfo & appInfo, std::vector<const char *> const & layers, std::vector<const char *> const & extensions);

	vk::DebugUtilsMessengerCreateInfoEXT makeDebugUtilsMessengerCreateInfoEXT();

	bool checkDeviceExtensionSupport(vk::PhysicalDevice device, const std::vector<const char *> & deviceExtensions);

	vk::UniqueShaderModule createShaderModule(vk::Device device, vk::ShaderStageFlagBits shaderStage, std::string const & shaderText);

	vk::UniqueSurfaceKHR createWindowSurface(vk::Instance instance, gen::Window const & window);

	std::string intToSemver(uint32_t version);

	void checkResult(vk::Result result);

	void insertImageMemoryBarrier(
		vk::CommandBuffer cmdbuffer,
		vk::Image image,
		vk::AccessFlags srcAccessMask,
		vk::AccessFlags dstAccessMask,
		vk::ImageLayout oldImageLayout,
		vk::ImageLayout newImageLayout,
		vk::PipelineStageFlags srcStageMask,
		vk::PipelineStageFlags dstStageMask,
		vk::ImageSubresourceRange subresourceRange);

} // namespace vk::utils
