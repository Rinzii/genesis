#pragma once
#include <cstdint>

namespace gen::logger {
///
/// \brief Log Level.
///
enum class Level : std::uint8_t { eError, eWarn, eInfo, eDebug, eCOUNT_ };

///
/// \brief char representation of Level.
///
constexpr char levelChar(Level const level) {
	switch (level) {
	case Level::eError: return 'E';
	case Level::eWarn: return 'W';
	case Level::eInfo: return 'I';
	case Level::eDebug: return 'D';
	default: return '?';
	}
}
} // namespace gen::logger
