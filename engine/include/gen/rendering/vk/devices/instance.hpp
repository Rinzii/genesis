// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/system/types.hpp"
#include "gen/rendering/vk/devices/physicalDevice.hpp"

#include <vulkan/vulkan.hpp>

#include <string>

namespace gen
{

	///
	/// \brief A wrapper of the vulkan instance.
	///
	class Instance
	{
	private:
		///
		/// \brief The minimum api version required by the engine.
		///
		/// \Note We are using the vulkan 1.3 api as internally the engine uses dynamic rendering.
		///
		static constexpr u32 s_minimumApiVersion = VK_API_VERSION_1_3;

	public:
		///
		/// \brief Initializes the vulkan instance.
		/// \param appName Name of the application.
		/// \param appVersion Version of the application.
		/// \param engineName Entered name of the engine. Should be "Genesis Engine".
		/// \param engineVersion Version of the engine.
		/// \param enableValidationLayers Should validation layers be enabled.
		/// \param requiredExtensions Desired extensions to be enabled.
		/// \param requiredLayers Desired layers to be enabled.
		/// \param apiVersion The api version of Vulkan to use.
		///
		Instance(const std::string & appName,
				 u32 appVersion,
				 const std::string & engineName,
				 u32 engineVersion,
				 bool enableValidationLayers = false,
				 std::vector<std::string> const & requiredExtensions = {},
				 std::vector<std::string> const & requiredLayers = {},
				 u32 apiVersion = VK_API_VERSION_1_3);

		///
		/// \brief Receives an already created vulkan instance and queries the physical devices.
		/// \param instance Initialized Vulkan instance.
		///
		explicit Instance(vk::Instance instance);

		~Instance() = default;

		Instance(const Instance &) = delete;
		Instance(Instance &&) = delete;
		Instance & operator=(const Instance &) = delete;
		Instance & operator=(Instance &&) = delete;

		///
		/// \brief Get the handle to the vulkan instance.
		/// \return vk::Instance The handle to the vulkan instance.
		///
		[[nodiscard]] vk::Instance getHandle() const { return m_handle.get(); }

		///
		/// \brief Get the enabled extensions.
		/// \return Vector of the enabled extensions.
		///
		[[nodiscard]] std::vector<const char *> const & getEnabledExtensions() const { return m_enabledExtensions; }

		///
		/// \brief Get the physical devices available on the system.
		/// \return Vector of the physical devices found by the instance.
		///
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

#if (defined(GEN_DEBUG) || defined(GEN_VK_FORCE_VALIDATION_LAYERS)) && !defined(GEN_VK_DISABLE_VALIDATION_LAYERS) && !(defined(GEN_NDEBUG) && !defined(GEN_VK_FORCE_VALIDATION_LAYERS))
		vk::UniqueDebugUtilsMessengerEXT debugUtilsMessengerExt;
#endif
	};
} // namespace gen