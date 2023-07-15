#pragma once
#include <logger/target.hpp>
#include <source_location>
#include <string_view>

namespace gen::refactor::logger {
using SrcLoc = std::source_location;

auto error(std::string_view category, std::string_view message, SrcLoc const& location = SrcLoc::current()) -> void;
auto warn(std::string_view category, std::string_view message, SrcLoc const& location = SrcLoc::current()) -> void;
auto info(std::string_view category, std::string_view message, SrcLoc const& location = SrcLoc::current()) -> void;
auto debug(std::string_view category, std::string_view message, SrcLoc const& location = SrcLoc::current()) -> void;

inline auto error(std::string_view message, SrcLoc const& location = SrcLoc::current()) -> void { error("general", message, location); }
inline auto warn(std::string_view message, SrcLoc const& location = SrcLoc::current()) -> void { warn("general", message, location); }
inline auto info(std::string_view message, SrcLoc const& location = SrcLoc::current()) -> void { info("general", message, location); }
inline auto debug(std::string_view message, SrcLoc const& location = SrcLoc::current()) -> void { debug("general", message, location); }
} // namespace gen::refactor::logger
