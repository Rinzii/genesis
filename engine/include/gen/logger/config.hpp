// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once
#include <cassert>
#include <string>
#include <unordered_map>
#include "gen/util/fixed_string.hpp"
#include "level.hpp"
#include "target.hpp"

namespace gen::logger
{
	/// \brief Timestamp mode.
	enum class Timestamp
	{
		eLocal,
		eUtc
	};

	struct Config
	{
		///
		/// \brief Default format specification for log entries.
		///
		/// Syntax: "{format_key}..."
		/// Text not matching any format keys will be passed through.
		///
		/// Supported format keys:
		///  level: log Level
		///  thread: current (logging) thread ID
		///  category: log category
		///  message: log message
		///  timestamp: log timestamp
		///
		static constexpr std::string_view default_format_v{"[{level}][T{thread}] [{category}] {message} [{timestamp}]"};

		///
		/// \brief Default format specification for verbose log entries.
		///
		/// Syntax: "{format_key}..."
		/// Text not matching any format keys will be passed through.
		///
		/// Supported format keys:
		///  level: log Level
		///  thread: current (logging) thread ID
		///  category: log category
		///  message: log message
		///  timestamp: log timestamp
		///  func: function name
		///  file: file name
		///  line: line number
		///
		static constexpr std::string_view verbose_format_v{"[{level}][T{thread}] [{category}] {message} [{timestamp}] [F: {func}] [{file}:{line}]"};

		static constexpr std::size_t format_size_v{128};

		static_assert(default_format_v.size() < format_size_v);
		static_assert(verbose_format_v.size() < format_size_v);

		/// \brief Format specification for log entries.
		FixedString<format_size_v> format{default_format_v};

		/// \brief Max log Level.
		Level maxLevel{Level::eDebug};

		/// \brief Max log Level overrides per category.
		std::unordered_map<std::string_view, Level> categoryMaxLevels{};

		/// \brief Log Target overrides per Level.
		std::unordered_map<Level, Target> levelTargets{};

		/// \brief Timestamp mode.
		Timestamp timestamp{Timestamp::eLocal};

		/// \brief Whether to print verbose logging information.
		bool verbose{false};
	};
} // namespace gen::logger
