// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"

#include <string>

// TODO: Replace this with a proper implementation

// Start Forward declarations

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>

// End Forward declarations

namespace gen {

class Window
{
public:
    Window(int w, int h, std::string title);
    ~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	// Member functions

	GEN_NODISCARD bool shouldClose() const;
	static void pollEvents() ;

	// Helper functions

	vk::SurfaceKHR createWindowSurface(vk::Instance & instance);


	// Getters

	GEN_NODISCARD inline int getWidth() const { return m_width; }
	GEN_NODISCARD inline int getHeight() const { return m_height; }
	GEN_NODISCARD inline const std::string& getTitle() const { return m_title; }
	GEN_NODISCARD inline GLFWwindow* getHandle() const { return m_window; }


	// TODO: Currently these functions will break the window. Have not yet implemented window resizing.

	void inline SetWidth(int width) { m_width = width; }
	void inline SetHeight(int height) { m_height = height; }
	void inline SetTitle(const std::string& title) { m_title = title; }

private:
	void init();

	int m_width;
	int m_height;
	std::string m_title;

	GLFWwindow* m_window;
};

} // namespace gen
