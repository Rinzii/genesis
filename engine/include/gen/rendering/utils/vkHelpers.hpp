// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <cstdint>
#include <iosfwd>
#include <map>

#include <vulkan/vulkan.hpp>

namespace vk::utils
{
	std::vector<char const *> gatherExtensions(
		std::vector<std::string> const & extensions
#ifndef GEN_NDEBUG
		,
		std::vector<vk::ExtensionProperties> const & extensionProperties
#endif
	);

	// Create an instance create info chain with or without debug utils messenger in debug mode.
	vk::StructureChain<vk::InstanceCreateInfo> makeInstanceCreateInfoChain(
		const vk::ApplicationInfo & appInfo, std::vector<const char *> const & layers, std::vector<const char *> const & extensions);

	void checkResult(vk::Result result);

	template <class T>
	void checkResult(T result)
	{
		checkResult(static_cast<vk::Result>(result));
	}

	/**
	 * @brief Allows unused vk:Result's with [[nodiscard]] to be discarded.
	 */
	void unusedResult(vk::Result result);

	std::string intToSemver(std::uint32_t version);

	std::string getDriverVendorName(std::uint32_t vendorID);

} // namespace vk::utils