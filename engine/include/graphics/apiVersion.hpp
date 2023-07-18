// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <compare>
#include <cstdint>
#include <vulkan/vulkan.hpp>


namespace gen
{
	///
	/// \brief Semantic Version
	///
	struct ApiVersion
	{
		std::uint16_t desiredMajor{};
		std::uint16_t desiredMinor{};
		std::uint32_t desiredVersion = makeApiVersion(0, desiredMajor, desiredMinor, 0);
		std::string desiredApiVersionString = std::to_string(desiredMajor) + "." + std::to_string(desiredMinor);



		auto operator <=> (ApiVersion const&) const = default;

		[[nodiscard]] constexpr std::uint32_t getRequestedApi() const
		{
			return desiredVersion;
		}

		// Copied from vulkan modified to not use macros
		[[nodiscard]] static constexpr std::uint32_t makeVersion(
			unsigned int major,
			unsigned int minor,
			unsigned int patch)
		{
			return static_cast<std::uint32_t>((major << 22U) | (minor << 12U) | (patch));
		}

		// Copied from vulkan modified to not use macros
		[[nodiscard]] static constexpr std::uint32_t makeApiVersion(
			unsigned int variant,
			unsigned int major,
			unsigned int minor,
			unsigned int patch)
		{
			return static_cast<std::uint32_t>((variant << 29U) | (major << 22U) | (minor << 12U) | (patch));
		}

		[[nodiscard]] constexpr bool isApiVersionSupported(std::uint32_t availableApiVersion)
        {
            return availableApiVersion >= makeApiVersion(0, desiredMajor, desiredMinor, 0);
        }


	};
} // namespace gen
