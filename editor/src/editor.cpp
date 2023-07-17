#include "editor.hpp"
//#include "backends/imgui_impl_vulkan.h" this include doesnt work because it can't find vulkan
#include "backends/imgui_impl_glfw.h"

namespace gen
{
	namespace editor
	{
		namespace
		{
			std::vector<Tab*> g_tabs;
		}

		void Init(GLFWwindow& window)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.IniFilename = nullptr;
			ImGui_ImplGlfw_InitForVulkan(&window, true);
			//ImGui_ImplVulkan_Init() TODO: Complete this part when the renderer will be further completed
		}

		void ShutDown()
		{
			//ImGui_ImplVulkan_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void AddEditorTab(Tab * tab)
		{
			g_tabs.push_back(tab);
		}

		void BeginFrame()
		{
			//ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void UpdateEditor()
		{
			for (const auto & g_tab : g_tabs)
				g_tab->Update();
		}
	}
}
