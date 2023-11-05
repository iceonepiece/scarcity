#include <imgui/imgui.h>
#include "imgui/imgui_stdlib.h"
#include "ImGuiBaseComponent.h"
#include "Components/BaseComponent.h"
#include "Core/Application.h"
#include "../Windows/ImGuiTagEditorWindow.h"
#include "../../EditorLayer.h"
#include "Core/TagManager.h"
#include "../ImGuiUtils.h"

void RenderImGui(BaseComponent& base)
{
	ImGui::Text("Name: "); ImGui::SameLine();
	ImGui::InputText("##name", &(base.name));

	auto& tagManager = Application::Get().GetTagManager();
	bool openPopup = false;

	ImGui::Text("Tag: "); ImGui::SameLine();

	if (ImGui::BeginCombo("##Tag", base.tag.c_str()))
	{
		ImGuiUtils::RenderTags(tagManager.GetDefaultTags(), base.tag);
		ImGuiUtils::RenderTags(tagManager.GetTags(), base.tag);

		ImGui::Separator();

		if (ImGui::Button("Add Tag..."))
		{
			openPopup = true;
		}

		ImGui::EndCombo();
	}

	if (openPopup)
		ImGui::OpenPopup("Tag Editor");

	if (ImGui::BeginPopupModal("Tag Editor"))
	{
		ImGuiTagEditorWindow* window = dynamic_cast<ImGuiTagEditorWindow*>(EditorLayer::GetImGuiWindow(ImGuiWindowType::Tags));
		window->Render();

		ImGui::Separator();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

}