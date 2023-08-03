#include "logger/log.hpp"
#include "logger/instance.hpp"

namespace gen
{
	// TODO: It may be better to instead set the logger to only log errors in release mode, but this should be discussed further.
#ifndef GEN_NDEBUG
    void logger::error(std::string_view category, std::string_view message, SrcLoc const& location)
	{
        Instance::print(message, Context::make(category, Level::eError, location));
    }

    void logger::warn(std::string_view category, std::string_view message, SrcLoc const& location)
	{
        Instance::print(message, Context::make(category, Level::eWarn, location));
    }

    void logger::info(std::string_view category, std::string_view message, SrcLoc const& location)
	{
        Instance::print(message, Context::make(category, Level::eInfo, location));
    }

    void logger::debug(std::string_view category, std::string_view message, SrcLoc const& location)
	{
        Instance::print(message, Context::make(category, Level::eDebug, location));
    }
#else // If GEN_NDEBUG is defined then diable logging. The creation of this symbol is created by cmake.
    void logger::error(std::string_view category, std::string_view message, SrcLoc const& location) {}
    void logger::warn(std::string_view category, std::string_view message, SrcLoc const& location) {}
    void logger::info(std::string_view category, std::string_view message, SrcLoc const& location) {}
    void logger::debug(std::string_view category, std::string_view message, SrcLoc const& location) {}
#endif
} // namespace gen
