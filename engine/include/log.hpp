// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <source_location>
#include "core.hpp"

namespace gen::logger {
std::ostream& trace(std::string const& message, std::source_location const& location = std::source_location::current());
std::ostream& log(std::string const& message, std::source_location const& location = std::source_location::current());
std::ostream& warn(std::string const& message, std::source_location const& location = std::source_location::current());
std::ostream& error(std::string const& message, std::source_location const& location = std::source_location::current());
std::ostream& fatal(std::string const& message, std::source_location const& location = std::source_location::current());
} // namespace gen::logger
