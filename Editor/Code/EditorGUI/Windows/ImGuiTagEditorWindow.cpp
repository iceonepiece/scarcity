#include "ImGuiTagEditorWindow.h"
#include "Core/Application.h"

ImGuiTagEditorWindow::ImGuiTagEditorWindow(EditorLayer& editor)
	: ImGuiWindow_(editor)
{

}

void ImGuiTagEditorWindow::Render()
{
	//ImGui::Begin("Tag Editor");

	auto& tags = Application::Get().GetTags();
	size_t defaultTagSize = Application::Get().GetDefaultTagSize();

	bool isSelected = false;

	for (int i = defaultTagSize; i < tags.size(); i++)
	{
		ImGui::PushID(("TAG_" + std::to_string(i)).c_str());
		
		if (ImGui::Selectable(tags[i].c_str(), isSelected)) {}

		ImGui::PopID();
	}

	//ImGui::End();
}