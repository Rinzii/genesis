// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/rendering/utils/vkHelpers.hpp"
#include "gen/logger/log.hpp"
#include "gen/rendering/utils/exceptions.hpp"

#include <numeric>

namespace vk::utils
{
	gen::Logger const logger{"vulkan"};

	// Gather enabled extensions based on the requested extensions and available extension properties.
	std::vector<char const *> gatherExtensions(
		std::vector<std::string> const & extensions
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
		availableExtensions.reserve(extensionProperties.size());
		for (auto const & eProp : extensionProperties) { availableExtensions.push_back(eProp.extensionName); }
#endif

		logger.debug(
			"Available extensions: \n\t{}",
			std::accumulate(
				availableExtensions.begin(),
				availableExtensions.end(),
				std::string(),
				[](const std::string & acc, const std::string & ext) { return acc.empty() ? ext : acc + ", \n\t" + ext; }));

		for (auto const & ext : extensions)
		{
			assert(std::any_of(
				extensionProperties.begin(), extensionProperties.end(), [ext](vk::ExtensionProperties const & eProp) { return ext == eProp.extensionName; }));

			enabledExtensions.push_back(ext.data());
		}

		return enabledExtensions;
	}

	// Create an instance create info chain with or without debug utils messenger in debug mode.
	vk::StructureChain<vk::InstanceCreateInfo> makeInstanceCreateInfoChain(
		const vk::ApplicationInfo & appInfo, std::vector<const char *> const & layers, std::vector<const char *> const & extensions)
	{

		// When in non-debug mode, use the InstanceCreateInfo for instance creation.
		vk::StructureChain<vk::InstanceCreateInfo> instanceCreateInfo({{}, &appInfo, layers, extensions});

		return instanceCreateInfo;
	}

	void checkResult(vk::Result result)
	{
		if (result != vk::Result::eSuccess)
		{
			logger.error("Detected Vulkan error: {}", vk::to_string(result));
			throw gen::VulkanException(vk::to_string(result));
		}
	}

	void unusedResult(vk::Result result)
	{
		static_cast<void>(result);
	}

	std::string intToSemver(std::uint32_t version)
	{
		std::uint32_t const major = (version >> 22U) & 127U;
		std::uint32_t const minor = (version >> 12U) & 1023U;
		std::uint32_t const patch = version & 4095U;

		return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
	}

	std::string getDriverVendorName(std::uint32_t vendorID)
	{
		// PCI vendor IDs that support vulkan
		// https://www.reddit.com/r/vulkan/comments/4ta9nj/comment/d5nso2t/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
		static std::map<std::uint32_t, std::string> const vendorNames{
			{0x1002, "AMD"},
			{0x1010, "ImgTec"},
			{0x10DE, "NVIDIA"},
			{0x13B5, "ARM"},
			{0x5143, "Qualcomm"},
			{0x8086, "INTEL"},
		};

		auto const it = vendorNames.find(vendorID);
		if (it != vendorNames.end()) { return it->second; }

		return "Unknown";
	}

} // namespace vk::utils