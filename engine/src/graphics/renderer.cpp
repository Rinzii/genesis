// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "graphics/renderer.hpp"
#include "graphics/graphicsExceptions.hpp"
#include "graphics/vkHelpers.hpp"

#ifndef GLFW_INCLUDE_NONE
	#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

namespace gen
{

	Renderer::Renderer(const Window & window, std::string const & appName)
		: m_device{m_instance.get(), m_surface.get()}, m_swapchain{window, m_device, m_surface.get()}
	{
		createInstance(appName, "Genesis Engine", VK_API_VERSION_1_3);
		createSurface(window);
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::createInstance(const std::string & appName, const std::string & engineName, const u32 & apiVersion)
	{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader const dynLoader;
		auto vkGetInstanceProcAddr = dynLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif

		// TODO: Allow for the application to be able to specify its version itself. Instead of just using the engine version.
		vk::ApplicationInfo const appInfo(appName.c_str(), gen::version_v.getVersion(), engineName.c_str(), gen::version_v.getVersion(), apiVersion);

		auto extensionsCount	   = 0U;
		auto * requestedExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);
		std::vector<std::string> const requestedExtensionsVec(
			requestedExtensions,
			requestedExtensions + extensionsCount); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		auto enabledExtensions = vk::util::gatherExtensions(
			requestedExtensionsVec
#ifndef GEN_NDEBUG
			,
			vk::enumerateInstanceExtensionProperties()
#endif
		);

		m_instance = vk::createInstanceUnique(vk::util::makeInstanceCreateInfoChain(appInfo, {}, enabledExtensions).get<vk::InstanceCreateInfo>());

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_instance);
#endif
	}

	void Renderer::createSurface(const Window & window)
	{
		m_surface = vk::util::createWindowSurface(m_instance.get(), window);
		m_logger.debug("Created surface");
	}
} // namespace gen