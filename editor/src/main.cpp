// Copyright (c) 2023-present Genesis Engine contributors (see LICENSE.txt)

#include <application.hpp>
#include <log.hpp>
#include <editor.hpp>

#include <iostream>
#include <stdexcept>

int main()
{
    gen::Application app;

	gen::logger::log("Hello, world!");
	//gen::editor::Init(app.GetWindow().GetGLFWWindow()); These make the program crash since ImGui_ImplVulkan isn't working for now
	//gen::editor::TestTab tab{};
	//gen::editor::AddEditorTab(&tab);

	try {
		while (!app.GetWindow().shouldClose()) {
			/*gen::editor::BeginFrame();*/
			gen::Window::pollEvents();
			/*gen::editor::UpdateEditor();*/
        }
	} catch (const std::exception& e) {
		gen::logger::fatal(e.what());
        return 1;
	}

	/*gen::editor::ShutDown();*/

    return 0;
}

