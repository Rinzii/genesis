// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "gen/application.hpp"

namespace gen
{
	class Game : public Application
	{
	public:
		Game(const char * appName, u32 appVersion, mim::vec2i const & initialSize);

		void draw() override;
		void update(float dt) override;
	};
} // namespace gen
