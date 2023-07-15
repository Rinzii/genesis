#pragma once
#include <logger/level.hpp>
#include <logger/target.hpp>
#include <util/fixed_string.hpp>
#include <cassert>
#include <string>
#include <unordered_map>

namespace gen::logger {
///
/// \brief Timestamp mode.
///
enum class Timestamp { eLocal, eUtc };

///
/// \brief Source location mode.
///
enum class Location { eFilename, eFullPath };

struct Config {
	///
	/// \brief Default format specification for log entries.
	///
	/// Syntax: "{format_key}..."
	/// Text not matching any format keys will be passed through.
	///
	/// Supported format keys:
	///  level: log Level
	///  thread: current (logging) thread ID
	///  message: log message
	///  timestamp: log timestamp
	///  location: log source location
	///
	static constexpr std::string_view default_format_v{"[{level}][T{thread}] {message} [{timestamp}] [{location}]"};

	static constexpr std::size_t format_size_v{64};

	static_assert(default_format_v.size() < format_size_v);

	///
	/// \brief Format specification for log entries.
	///
	FixedString<format_size_v> format{default_format_v};
	///
	/// \brief Max log Level.
	///
	Level maxLevel{Level::eDebug};
	///
	/// \brief Max log Level overrides per category.
	///
	std::unordered_map<std::string_view, Level> categoryMaxLevels{};
	///
	/// \brief Log Target overrides per Level.
	///
	std::unordered_map<Level, Target> levelTargets{};
	///
	/// \brief Timestamp mode.
	///
	Timestamp timestamp{Timestamp::eLocal};
	///
	/// \brief Source location mode.
	///
	Location location{Location::eFilename};
};
} // namespace gen::logger
