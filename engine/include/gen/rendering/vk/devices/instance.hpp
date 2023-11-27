// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"
#include "gen/rendering/vk/devices/physicalDevice.hpp"

#include <vulkan/vulkan.hpp>

#include <string>

namespace gen
{

	class Instance
	{
	private:
		static constexpr u32 s_minimumApiVersion = VK_API_VERSION_1_3;

	public:
		Instance(const std::string & appName,
				 u32 appVersion,
				 const std::string & engineName,
				 u32 engineVersion,
				 bool enableValidationLayers = false,
				 std::vector<std::string> const & requiredExtensions = {},
				 std::vector<std::string> const & requiredLayers = {},
				 u32 apiVersion = VK_API_VERSION_1_3);

		explicit Instance(vk::Instance instance);

		~Instance();

		Instance(const Instance &) = delete;
		Instance(Instance &&) = delete;
		Instance & operator=(const Instance &) = delete;
		Instance & operator=(Instance &&) = delete;


		[[nodiscard]] vk::Instance getHandle() const { return m_handle.get(); }
		[[nodiscard]] std::vector<const char *> const & getEnabledExtensions() const { return m_enabledExtensions; }
		[[nodiscard]] std::vector<std::unique_ptr<PhysicalDevice>> const & getPhysicalDevices() const { return m_gpus; }



	private:

		///
		/// \brief Query the physical devices available on the system and store them in m_gpus.
		///
		void queryPhysicalDevices();

		///
		/// \brief Check if the provided api version meets the minimum required version.
		/// \param apiVersion The api version to check.
		///
		static void doesProvidedApiMeetMin(u32 apiVersion);


	private:
		vk::UniqueInstance m_handle{nullptr};

		std::vector<const char *> m_enabledExtensions{};



		std::vector<std::unique_ptr<PhysicalDevice>> m_gpus{};


#if (defined(GEN_DEBUG) || defined(GEN_VK_FORCE_VALIDATION_LAYERS)) && !defined(GEN_VK_DISABLE_VALIDATION_LAYERS)
		vk::UniqueDebugUtilsMessengerEXT debugUtilsMessengerExt;
#endif
	};
} // namespace gen