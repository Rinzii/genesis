// Copyright Ian Pike. All Rights Reserved.

#pragma once

#include "windowing/window.hpp"

namespace gen {

class Application {
public:

    void run();

	// This is all kinda bad but it is gonna be replaced later
	// Just using it for testing the rendering pipeline
public:
	static constexpr u32 m_width = 800;
	static constexpr u32 m_height = 600;

	Window& GetWindow();

private:
	Window m_window{ 800, 600, "Genesis Engine" };

};

} // namespace gen
