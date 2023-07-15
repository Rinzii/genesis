#pragma once
#include <logger/level.hpp>
#include <chrono>
#include <source_location>
#include <string_view>

namespace gen::refactor::logger {
using Clock = std::chrono::system_clock;
using SrcLoc = std::source_location;

enum struct ThreadId : int {};

struct Context {
	std::string_view category{};
	Clock::time_point timestamp{};
	std::source_location location{};
	ThreadId thread{};
	Level level{};

	static ThreadId getThreadId();
	static Context make(std::string_view category, Level level, SrcLoc const& location = SrcLoc::current());
};
} // namespace gen::refactor::logger
