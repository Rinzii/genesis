// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#pragma once

#include "core.hpp"

#include <string>

// TODO: Replace this with a proper implementation

// Forward declare GLFWwindow
struct GLFWwindow;

namespace gen {

class Window {
public:
    Window(int w, int h, std::string title);
    ~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;


public:
	GEN_NODISCARD bool shouldClose() const;
	static void pollEvents() ;


public:
	GEN_NODISCARD inline int GetWidth() const { return m_width; }
	GEN_NODISCARD inline int GetHeight() const { return m_height; }
	GEN_NODISCARD inline const std::string& GetTitle() const { return m_title; }


	// TODO: Currently these functions will break the window.

	void inline SetWidth(int width) { m_width = width; }
	void inline SetHeight(int height) { m_height = height; }
	void inline SetTitle(const std::string& title) { m_title = title; }

private:
	void initWindow();

private:
	int m_width;
	int m_height;
	std::string m_title;

	GLFWwindow* m_window;
};

} // namespace gen
