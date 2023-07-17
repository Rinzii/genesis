#pragma once
#include "windowing/window.hpp"
#include "tab.hpp"

namespace gen
{
	namespace editor
	{
		void Init(GLFWwindow& window);
		void ShutDown();

		void AddEditorTab(Tab* tab);

		void BeginFrame();
		void UpdateEditor();
	}
}
