// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once
#include <logger/level.hpp>
#include <chrono>
#include <optional>
#include <string_view>

namespace gen::logger
{
	using Clock = std::chrono::system_clock;

	///
	/// \brief Strongly typed integer representing logging thread ID.
	///
	enum struct ThreadId : int
	{
	};

	///
	/// \brief Log context.
	///
	/// Expected to be built at the call site and passed around.
	///
	struct Context
	{
		std::string_view category{};
		Clock::time_point timestamp{};
		ThreadId thread{};
		Level level{};
		std::optional<std::string_view> func{};
		std::optional<std::string_view> file{};
		std::optional<int> line{};

		///
		/// \brief Obtain this thread's ID.
		/// \returns Monotonically increasing IDs per thread, in order of being called for the first time.
		///
		static ThreadId getThreadId();
		static Context make(std::string_view category, Level level);
		static Context make(std::string_view category, Level level, std::string_view function, std::string_view filePath, int currentLine);
	};
} // namespace gen::logger
