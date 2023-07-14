// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"
#include "windowing/window.hpp"
#include "graphics/pipeline.hpp"

namespace gen {

class Application {
public:

    void run();

private:
	void init();
	void gameLoop();
	void shutdown();

private:
	void initializeGraphics();

public:
	static constexpr u32 m_width = 800;
	static constexpr u32 m_height = 600;

private:
	Window m_window{ m_width, m_height, "Genesis Engine" };
	GraphicsPipeline m_graphicsPipeline{ };

};

} // namespace gen
