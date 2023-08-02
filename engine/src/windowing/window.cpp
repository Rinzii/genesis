// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include "windowing/window.hpp"

#include "graphics/device.hpp"
#include "logger/log.hpp"

#include <GLFW/glfw3.h>


#include <utility>

namespace gen {

    Window::Window(int w, int h, std::string title) : m_width{w}, m_height{h}, m_title{std::move(title)} { // NOLINT(cppcoreguidelines-pro-type-member-init)
        init();
    }

	Window::~Window() {
        glfwDestroyWindow(m_window);
		glfwTerminate();
    }

	bool Window::shouldClose() const {
        return glfwWindowShouldClose(m_window) != 0;
    }

	void Window::pollEvents() {
        glfwPollEvents();
    }

	void Window::init() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO: Later add support for resizing of windows.

        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

    }

	vk::SurfaceKHR Window::createWindowSurface(vk::Instance & instance)
	{
		VkSurfaceKHR _surface = nullptr;
		VkResult const err = glfwCreateWindowSurface( static_cast<VkInstance>( instance ) , m_window, nullptr, &_surface );
		if ( err != VK_SUCCESS )
		{
			gen::logger::error( "Failed to create window surface!" );
			throw std::runtime_error( "Failed to create window!" );
		}
		return vk::SurfaceKHR( _surface ); // NOLINT(modernize-return-braced-init-list)
    }

} // namespace gwn
