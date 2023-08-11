#include "logger/log.hpp"
#include "logger/instance.hpp"

namespace gen
{
	void logger::print(logger::Level level, std::string_view category, std::string_view message)
	{
		Instance::print(message, Context::make(category, level));
	}

	Logger::Logger(std::string_view const category) : m_category(category.empty() ? "unknown" : category)
	{
	}
} // namespace gen
