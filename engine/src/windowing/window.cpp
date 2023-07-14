// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "windowing/window.hpp"

#include <GLFW/glfw3.h>

#include <utility>

namespace gen {

    Window::Window(int w, int h, std::string title) : m_width{w}, m_height{h}, m_title{std::move(title)} { // NOLINT(cppcoreguidelines-pro-type-member-init)
        initWindow();
    }

	Window::~Window() {
        glfwDestroyWindow(m_window);
    }

	bool Window::shouldClose() const {
        return glfwWindowShouldClose(m_window);
    }

	void Window::pollEvents() {
        glfwPollEvents();
    }

	void Window::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO: Later add support for resizing of windows.

        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

    }



} // namespace gwn
