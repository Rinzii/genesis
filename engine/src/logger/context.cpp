// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "logger/context.hpp"
#include <atomic>

namespace gen::logger
{
	ThreadId Context::getThreadId()
	{
		auto const get_next_id = []
		{
			static auto s_prevId{std::atomic<int>{}};
			return s_prevId++;
		};
		thread_local auto const s_thisThreadId{get_next_id()};
		return ThreadId{s_thisThreadId};
	}

	Context Context::make(std::string_view category, Level level)
	{
		return Context{
			.category  = category,
			.timestamp = Clock::now(),
			.thread	   = getThreadId(),
			.level	   = level,
		};
	}

	Context Context::make(std::string_view category, Level level, std::string_view function, std::string_view filePath, int currentLine)
	{
		return Context{
			.category  = category,
			.timestamp = Clock::now(),
			.thread	   = getThreadId(),
			.level	   = level,
			.func	   = function,
			.file	   = filePath,
			.line	   = currentLine,
		};
	}
} // namespace gen::logger
