#pragma once
#include <cstdint>

namespace gen::logger
{
	///
	/// \brief Log Target (destination).
	///
	struct Target
	{
		std::uint32_t value{};

		constexpr operator std::uint32_t() const { return value; }
	};

	///
	/// \brief Standard IO streams.
	///
	inline constexpr Target console_v{.value = 1 << 0};
	///
	/// \brief Log file.
	///
	inline constexpr Target file_v{.value = 1 << 1};
	///
	/// \brief Custom sinks.
	///
	inline constexpr Target sinks_v{.value = 1 << 2};

	inline constexpr Target all_v{.value = 0xffffffff};
} // namespace gen::logger
