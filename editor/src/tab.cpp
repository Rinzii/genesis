#include "tab.hpp"

gen::editor::Tab::Tab()
{
}

gen::editor::Tab::~Tab()
{
}

float gen::editor::Tab::GetWidth() const
{
	return m_width;
}

float gen::editor::Tab::GetHeight() const
{
	return m_height;
}

void gen::editor::TestTab::Update()
{
	ImGui::Begin("Demo window");
    ImGui::Button("Hello world!");
    ImGui::End();
}
