// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "application.hpp"

namespace gen
{
	class Game : public Application
	{
	public:
		Game(const char * appName, mim::vec2i const & initialSize);
		~Game() = default;

		void run();
		void draw();
		void update(double dt);
	};
} // namespace gen
