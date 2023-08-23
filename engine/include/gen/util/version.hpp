// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once
#include <compare>
#include "gen/system/types.hpp"

namespace gen
{
	///
	/// \brief Semantic Version
	///
	struct Version
	{
		u32 major{};
		u32 minor{};
		u32 patch{};

		auto operator<=>(Version const &) const = default;

		[[nodiscard]] constexpr std::uint32_t getVersion() const { return (major << 22U) | (minor << 12U) | (patch); }
	};
} // namespace gen
