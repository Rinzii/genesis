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

	///
	/// \brief Template helper for validating a template only accepts only integrals.
	//
	template <typename T>
	concept IsIntegral = std::is_integral_v<T>;

	///
	/// \brief Creates a valid semver value that is valid for Vulkan.
	/// \note Generally variant is not used by Vulkan, but is included for futures Vulkan as this is how the API now defines it.
	/// \note We only accept integrals with this function. We theoretically could allow anything and just convert it, but personally I think its just better to
	/// enforce that we only use integrals.
	///
	template <IsIntegral T>
	uint32_t makeVkVersion(T variant, T major, T minor, T patch)
	{
		// NOLINTNEXTLINE
		return (
			((static_cast<uint32_t>(variant)) << 29U) | ((static_cast<uint32_t>(major)) << 22U) | ((static_cast<uint32_t>(minor)) << 12U) |
			(static_cast<uint32_t>(patch)));
	}

	///
	/// \brief Creates a valid semver value that is valid for Vulkan.
	/// \note We only accept integrals with this function. We theoretically could allow anything and just convert it, but personally I think its just better to
	/// enforce that we only use integrals.
	///
	template <IsIntegral T>
	uint32_t makeVkVersion(T major, T minor, T patch)
	{
		// NOLINTNEXTLINE
		return ((static_cast<uint32_t>(major)) << 22U) | ((static_cast<uint32_t>(minor)) << 12U) | (static_cast<uint32_t>(patch)));
	}

	std::string intToSemver(std::uint32_t version);

	std::string getDriverVendorName(std::uint32_t vendorID);

} // namespace vk::utils