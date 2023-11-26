// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/instance.hpp"
#include "gen/genesis_version.hpp"
#include "gen/rendering/utils/exceptions.hpp"
#include "gen/rendering/utils/vkHelpers.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
// NOLINTNEXTLINE
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

namespace gen
{

	Instance::Instance(const std::string & appName, u32 appVersion, const std::string & engineName, u32 apiVersion)
	{
		// TODO: Currently we only support Vulkan 1.3+. Maybe at a later date we will allow a lower vulkan version, but for now validate that we are using 1.3+.
		assert(apiVersion >= VK_API_VERSION_1_3 && "Vulkan API version must be at least 1.3");

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader const dynLoader;
		auto vkGetInstanceProcAddr = dynLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif

		vk::ApplicationInfo const appInfo(appName.c_str(), appVersion, engineName.c_str(), gen::version_v.getVersion(), apiVersion);

		// Get required extensions by glfw
		auto extensionsCount		  = 0U;
		auto * requiredGlfwExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);

		std::vector<std::string> const requiredGlfwExtensionsVec(
			requiredGlfwExtensions, requiredGlfwExtensions + extensionsCount); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

		auto enabledExtensions = vk::utils::gatherExtensions(
			requiredGlfwExtensionsVec
#if !defined(GEN_NDEBUG) || defined(GEN_FORCE_VALIDATION_LAYERS)
			,
			vk::enumerateInstanceExtensionProperties()
#endif
		);

		m_handle = vk::createInstanceUnique(vk::utils::makeInstanceCreateInfoChain(appInfo, {}, enabledExtensions).get<vk::InstanceCreateInfo>());

		if (!m_handle) { throw gen::VulkanException("Failed to create instance!"); }

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_handle);
#endif
	}

	Instance::Instance(vk::UniqueInstance instance)
	{
	}
	Instance::Instance(vk::Instance instance)
	{
	}

} // namespace gen