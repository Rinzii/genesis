// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "windowing/window.hpp"

#include <format>
#include "graphics/device.hpp"
#include "logger/log.hpp"

#include <GLFW/glfw3.h>

#include <utility>

namespace gen
{

	Window::Window(int width, int height, std::string title)
		: m_width{width}, m_height{height}, m_title{std::move(title)} // NOLINT(cppcoreguidelines-pro-type-member-init)
	{
		if (!glfwInit()) // NOLINT(readability-implicit-bool-conversion)
		{
			gen::logger::error("windowing", "Failed to initialize GLFW");
			throw std::runtime_error("Failed to initialize GLFW");
		}

		// Set window hints
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO: Later add support for resizing of windows.

		// Set callbacks
		glfwSetErrorCallback(callback_error);

		// Create window
		GLFWwindow * window_ = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
		m_window			 = std::shared_ptr<GLFWwindow>(window_,
											   [](GLFWwindow * window)
											   {
												   glfwDestroyWindow(window);
												   glfwTerminate();
											   });

		// Check if window was created
		if (!m_window)
		{
			gen::logger::error("windowing", "Failed to create GLFW window");
			throw std::runtime_error("Failed to create GLFW window");
		}

		// Report successful window creation
		gen::logger::info("windowing", "Window instance constructed");
	}

	Window::~Window()
	{
		// This is here for simple information and may be removed later if we don't need to destruct anything later.
		gen::logger::info("windowing", "Window instance destructed");
	}

	bool Window::shouldClose() const
	{
		return glfwWindowShouldClose(m_window.get()) != 0;
	}

	void Window::pollEvents()
	{
		glfwPollEvents();
	}

	// Callbacks

	void Window::callback_error(int error, const char * description)
	{
		gen::logger::warn("windowing", std::format("GLFW Error: {} - {}", error, description));
	}

} // namespace gen
