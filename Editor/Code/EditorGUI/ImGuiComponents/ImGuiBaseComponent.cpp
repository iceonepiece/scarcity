#include <imgui/imgui.h>
#include "imgui/imgui_stdlib.h"
#include "ImGuiBaseComponent.h"
#include "Components/BaseComponent.h"
#include "Core/Application.h"
#include "../Windows/ImGuiTagEditorWindow.h"
#include "../../EditorLayer.h"

void RenderImGui(BaseComponent& base)
{
	ImGui::Text("Name: "); ImGui::SameLine();
	ImGui::InputText("##name", &(base.name));

	auto& tags = Application::Get().GetTags();
	bool openPopup = false;

	ImGui::Text("Tag: "); ImGui::SameLine();

	if (ImGui::BeginCombo("##Tag", base.tag > 0 ? tags[base.tag - 1].c_str() : "Untagged"))
	{
		for (int i = 0; i <= tags.size(); i++)
		{
			bool isSelected = base.tag == i;

			ImGui::PushID(("TAG_" + std::to_string(i)).c_str());
			if (i == 0)
			{
				if (ImGui::Selectable("Untagged", isSelected))
					base.tag = i;
			}
			else
			{
				if (ImGui::Selectable(tags[i - 1].c_str(), isSelected))
					base.tag = i;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();

			ImGui::PopID();
		}

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

		ImGui::EndPopup();
	}

}