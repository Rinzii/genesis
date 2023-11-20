// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "logger/log.hpp"
#include "logger/instance.hpp"

namespace gen
{
	void logger::print(logger::Level level, std::string_view category, std::string_view message)
	{
		Instance::print(message, Context::make(category, level));
	}

	void logger::print(
		logger::Level level, std::string_view category, std::string_view function, std::string_view filePath, int curLine, std::string_view message)
	{
		Instance::print(message, Context::make(category, level, function, filePath, curLine));
	}

	Logger::Logger(std::string_view const category) : m_category(category.empty() ? "unknown" : category)
	{
	}
} // namespace gen
