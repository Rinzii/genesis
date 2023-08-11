// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

// internal
#include "windowing/window.hpp"
#include "logger/log.hpp"

// external
#include <GLFW/glfw3.h>

// std
#include <format>
#include <cassert>

namespace gen
{

	Window::Window(const mim::vec2i extent, const char* title) // NOLINT(performance-unnecessary-value-param)
		: m_extent{extent} // NOLINT(cppcoreguidelines-pro-type-member-init)
	{

		if (!glfwInit()) // NOLINT(readability-implicit-bool-conversion)
		{
			gen::logger::error("windowing", "Failed to initialize GLFW");
			throw std::runtime_error("Failed to initialize GLFW");
		}

		// Set window hints
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO: Later add support for resizing of windows.

		// Create window
		GLFWwindow * window_ = glfwCreateWindow(static_cast<int>(m_extent.x),
												static_cast<int>(m_extent.y),
												title,
												nullptr,
												nullptr);

		m_window = std::unique_ptr<GLFWwindow, Deleter>(window_);

		// Check if window was created
		if (!m_window)
		{
			gen::logger::error("windowing", "Failed to create GLFW window");
			throw std::runtime_error("Failed to create GLFW window");
		}

		// Set modes
		glfwSetInputMode(m_window.get(), GLFW_CURSOR, static_cast<int>(m_currentCursorMode));

		// Set callbacks
		glfwSetErrorCallback(callback_error);

		// Report successful window creation
		gen::logger::info("windowing", "Window constructed");
	}

	Window::~Window()
	{
		gen::logger::info("windowing", "Window destructed");
	}

	bool Window::shouldClose() const
	{
		return glfwWindowShouldClose(m_window.get()) != 0;
	}

	void Window::pollEvents()
	{
		glfwPollEvents();
	}

	/// Getters

	mim::vec2i Window::getExtent()
	{
		// Query glfw for the window size just in case it has changed
		glfwGetWindowSize(m_window.get(), &m_extent.x, &m_extent.y);
		return m_extent;
	}

	int Window::getWidth()
	{
		// Query glfw for the window size just in case it has changed
		glfwGetWindowSize(m_window.get(), &m_extent.x, &m_extent.y);
		return m_extent.x;
	}

	int Window::getHeight()
	{
		// Query glfw for the window size just in case it has changed
		glfwGetWindowSize(m_window.get(), &m_extent.x, &m_extent.y);
		return m_extent.y;
	}

	Window::CursorMode Window::getCursorMode() const
	{
		return m_currentCursorMode;
	}

	GLFWwindow * Window::getHandle() const
	{
		return m_window.get();
	}


	/// Setters

	void Window::setWidth(const int width)
	{
		assert(width > 0);
		m_extent.x = width;
		glfwSetWindowSize(m_window.get(), m_extent.x, m_extent.y);
	}

	void Window::setHeight(const int height)
	{
		assert(height > 0);
		m_extent.y = height;
		glfwSetWindowSize(m_window.get(), m_extent.x, m_extent.y);
	}

	void Window::setTitle(const char* const title)
	{
		glfwSetWindowTitle(m_window.get(), title);
	}

	void Window::setCursorMode(const Window::CursorMode mode)
	{
		m_currentCursorMode = mode;
		glfwSetInputMode(m_window.get(), GLFW_CURSOR, static_cast<int>(m_currentCursorMode));
	}

	/// Callbacks

	void Window::callback_error(int error, const char * description)
	{
		gen::logger::error("windowing", std::format("GLFW Error: {} - {}", error, description));
	}



	void Window::Deleter::operator()(GLFWwindow * ptr) const
	{
		glfwDestroyWindow(ptr);
		glfwTerminate();
	}
} // namespace gen
