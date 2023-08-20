// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include <stdexcept>
#include <string>

namespace gen
{
	struct windowing_error : public std::runtime_error
	{

		explicit windowing_error(const std::string & message);
	};
} // namespace gen
