// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"

#include <string>

// TODO: Replace this with a proper implementation

// Start Forward declarations
struct GLFWwindow;

// End Forward declarations

namespace gen {

class Window
{
public:
    Window(int width, int height, std::string title);
    ~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	// Member functions

	GEN_NODISCARD bool shouldClose() const;
	static void pollEvents() ;



	// Getters

	GEN_NODISCARD int getWidth() const { return m_width; }
	GEN_NODISCARD int getHeight() const { return m_height; }
	GEN_NODISCARD const std::string& getTitle() const { return m_title; }
	GEN_NODISCARD GLFWwindow* getHandle() const { return m_window; }


	// TODO: Currently these functions will break the window. Have not yet implemented window resizing.

	void SetWidth(int width) { m_width = width; }
	void SetHeight(int height) { m_height = height; }
	void SetTitle(const std::string& title) { m_title = title; }

private:
	void init();

	int m_width;
	int m_height;
	std::string m_title;

	GLFWwindow* m_window;
};

} // namespace gen
