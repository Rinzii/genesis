// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"
#include "windowing/window.hpp"
#include "graphics/pipeline.hpp"
#include "graphics/device.hpp"

namespace gen {

class Application {
public:

    void run();

private:
	void initialize();
	void gameLoop();
	void shutdown();


	// Initialization functions

	void initializeGraphics();

public:
	static const u32 m_width = 800;
	static const u32 m_height = 600;

private:
	Window m_window { m_width, m_height, "Genesis Engine" };
	GraphicsDevice m_graphicsDevice { m_window };
	GraphicsPipeline m_graphicsPipeline { m_graphicsDevice , GraphicsPipeline::defaultPipelineConfigInfo(m_width, m_height)};

};

} // namespace gen
