#pragma once
#include <compare>
#include <cstdint>

namespace gen
{
	///
	/// \brief Semantic Version
	///
	struct Version
	{
		std::uint32_t major{};
		std::uint32_t minor{};
		std::uint32_t patch{};

        auto operator <=> (Version const&) const = default;

		[[nodiscard]] constexpr std::uint32_t getVersion() const
		{
			return (major << 22U) | (minor << 12U) | (patch);
		}

    };
} // namespace gen
