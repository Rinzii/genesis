// Copyright Ian Pike. All Rights Reserved.

#include "graphics/pipeline.hpp"
#include "core.hpp"
#include "graphics/vkHelpers.hpp"
#include "log.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace gen
{
	GraphicsPipeline::GraphicsPipeline()
	{
		createGraphicsPipeline();
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		destroyGraphicsPipeline();
	}

	std::vector<char> GraphicsPipeline::readFile(const std::string & filePath)
	{
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) { throw std::runtime_error("failed to open file: " + filePath); }

		std::size_t fileSize = static_cast<size_t>(file.tellg());

		return std::vector<char>();
	}

	void GraphicsPipeline::createGraphicsPipeline()
	{
		createInstance("Genesis Engine", "Genesis Engine", {}, {}, VK_API_VERSION_1_0);
		createDebugMessenger();
		gen::logger::log("GraphicsPipeline created");
	}

	void GraphicsPipeline::destroyGraphicsPipeline()
	{
#if !defined(NDEBUG) || !defined(GEN_NDEBUG)
		m_instance.destroyDebugUtilsMessengerEXT(m_debugMessenger);
#endif

		m_instance.destroy();
	}

	void GraphicsPipeline::createInstance(std::string const & appName, std::string const & engineName, std::vector<std::string> const & layers,
										  std::vector<std::string> const & extensions, u32 const & apiVersion)
	{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader dl;
		auto vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#endif

		vk::ApplicationInfo appInfo(appName.c_str(), gen::version_v.getVersion(), engineName.c_str(), gen::version_v.getVersion(), apiVersion);

		vk::InstanceCreateInfo createInfo({}, &appInfo);

		auto enabledLayers = vk::util::gatherLayers(layers
#if !defined(NDEBUG) || !defined(GEN_NDEBUG)
													,
													vk::enumerateInstanceLayerProperties()
#endif
		);

		auto enabledExtensions = vk::util::gatherExtensions(extensions
#if !defined(NDEBUG) || !defined(GEN_NDEBUG)
															,
															vk::enumerateInstanceExtensionProperties()
#endif
		);

		m_instance = vk::createInstance(vk::util::makeInstanceCreateInfoChain(appInfo, enabledLayers, enabledExtensions).get<vk::InstanceCreateInfo>());

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// initialize function pointers for instance
		VULKAN_HPP_DEFAULT_DISPATCHER.init(m_instance);
#endif
	}

	void GraphicsPipeline::createDebugMessenger()
	{
#if !defined(NDEBUG) || !defined(GEN_NDEBUG)
		m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(vk::util::makeDebugUtilsMessengerCreateInfoEXT());
#endif
	}

} // namespace gen
