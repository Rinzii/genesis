#include "logger/log.hpp"
#include "logger/instance.hpp"

namespace gen::refactor {
auto logger::error(std::string_view category, std::string_view message, SrcLoc const& location) -> void {
	Instance::print(message, Context::make(category, Level::eError, location));
}

auto logger::warn(std::string_view category, std::string_view message, SrcLoc const& location) -> void {
	Instance::print(message, Context::make(category, Level::eWarn, location));
}

auto logger::info(std::string_view category, std::string_view message, SrcLoc const& location) -> void {
	Instance::print(message, Context::make(category, Level::eInfo, location));
}

auto logger::debug(std::string_view category, std::string_view message, SrcLoc const& location) -> void {
	Instance::print(message, Context::make(category, Level::eDebug, location));
}
} // namespace gen::refactor
