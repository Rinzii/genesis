#include "logger/log.hpp"
#include "logger/instance.hpp"

namespace gen
{
	void logger::error(std::string_view category, std::string_view message, SrcLoc const & location)
	{
		Instance::print(message, Context::make(category, Level::eError, location));
	}

	void logger::warn(std::string_view category, std::string_view message, SrcLoc const & location)
	{
		Instance::print(message, Context::make(category, Level::eWarn, location));
	}

	void logger::info(std::string_view category, std::string_view message, SrcLoc const & location)
	{
		Instance::print(message, Context::make(category, Level::eInfo, location));
	}

	void logger::debug(std::string_view category, std::string_view message, SrcLoc const & location)
	{
		Instance::print(message, Context::make(category, Level::eDebug, location));
	}
} // namespace gen
