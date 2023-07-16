// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <source_location>
#include "core.hpp"

namespace gen::logger
{
	std::ostream & trace(const std::string & message, const std::source_location & location = std::source_location::current());
	std::ostream & log(const std::string & message, const std::source_location & location = std::source_location::current());
	std::ostream & warn(const std::string & message, const std::source_location & location = std::source_location::current());
	std::ostream & error(const std::string & message, const std::source_location & location = std::source_location::current());
	std::ostream & fatal(const std::string & message, const std::source_location & location = std::source_location::current());
} // namespace gen::logger
