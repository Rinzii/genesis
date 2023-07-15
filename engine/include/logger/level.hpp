#pragma once
#include <cstdint>

namespace gen::refactor::logger {
///
/// \brief Log Level.
///
enum class Level : std::uint8_t { eError, eWarn, eInfo, eDebug, eCOUNT_ };

///
/// \brief char representation of Level.
///
constexpr auto levelChar(Level const level) -> char {
	switch (level) {
	case Level::eError: return 'E';
	case Level::eWarn: return 'W';
	case Level::eInfo: return 'I';
	case Level::eDebug: return 'D';
	default: return '?';
	}
}
} // namespace gen::refactor::logger
