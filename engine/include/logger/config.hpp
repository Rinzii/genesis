#pragma once
#include <logger/level.hpp>
#include <logger/target.hpp>
#include <cassert>
#include <string>
#include <unordered_map>

namespace gen::refactor::logger {
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

	///
	/// \brief Format specification for log entries.
	///
	std::string format{default_format_v};
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
} // namespace gen::refactor::logger
