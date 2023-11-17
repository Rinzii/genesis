// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

namespace gen
{

	class Target
	{
	public:
		Target()  = default;
		~Target() = default;

		Target(const Target &)			   = delete;
		Target(Target &&)				   = delete;
		Target & operator=(const Target &) = delete;
		Target & operator=(Target &&)	   = delete;

	private:
	};
} // namespace gen