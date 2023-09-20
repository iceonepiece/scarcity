#pragma once

#include <imgui/imgui.h>
#include "imgui/imgui_stdlib.h"
#include "Components/BaseComponent.h"
#include "Core/Application.h"

static void RenderImGui(BaseComponent& base)
{
	ImGui::Text("Name: "); ImGui::SameLine();
	ImGui::InputText("##name", &(base.name));

	auto& tags = Application::Get().GetTags();

	ImGui::Text("Tag: "); ImGui::SameLine();

	if (ImGui::BeginCombo("##Tag", base.tag > 0 ? tags[base.tag - 1].c_str() : "Untagged"))
	{
		for (int i = 0; i <= TAG_SIZE; i++)
		{
			bool isSelected = base.tag == i;

			ImGui::PushID(("TAG_" + std::to_string(i)).c_str());
			if (i == 0)
			{
				if (ImGui::Selectable("Untagged", isSelected))
					base.tag = i;

				ImGui::Separator();
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

		ImGui::EndCombo();
	}

}