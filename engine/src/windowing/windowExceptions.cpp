// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "gen/windowing/windowExceptions.hpp"

#include "gen/logger/log.hpp"

namespace gen
{
	windowing_error::windowing_error(const std::string & message) : std::runtime_error{message}
	{
		Logger const log{"windowing"};
		log.error("{}", message);
	}

} // namespace gen
