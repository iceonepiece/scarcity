#include "ImGui_AnimatorInspector.h"
#include "../ImGui_AnimatorPanel.h"
#include "Animations/AnimatorState.h"
#include "../ImGuiUtils.h"
#include <imgui/imgui_stdlib.h>

void ImGui_AnimatorInspector::Render(AnimatorState& state)
{
	ImGui::InputText("Title", &state.m_name);

	const char* motions[] = { "None", "Square", "Circle" };
	int selectedIndex = 0;

	if (ImGui::BeginCombo("Shape Type", motions[selectedIndex]))
	{
		for (int i = 0; i < 3; i++)
		{
			bool isSelected = i == selectedIndex;
			if (ImGui::Selectable(motions[i], isSelected))
			{
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::InputFloat("Speed", &state.m_speed);

	ImGui::Separator();
	ImGui::Text("Transitions");

	bool isSelected = false;

	for (auto& transition : state.GetOutgoingTransitions())
	{
		std::string transitionName = transition->GetFromState()->m_name + " -> " + transition->GetNextState()->m_name;

		if (ImGui::Selectable(transitionName.c_str(), m_selectedTransition == transition))
		{
			m_selectedTransition = transition;
		}

		if (m_selectedTransition == transition)
			isSelected = true;
	}

	if (isSelected && m_selectedTransition != nullptr)
	{
		if (ImGui::Button("Add condition"))
		{
			ImGui::OpenPopup("AddConditionPopup");
		}

		if (ImGui::BeginPopup("AddConditionPopup"))
		{
			for (auto& parameter : state.GetController()->GetParameters())
			{
				if (ImGui::MenuItem(parameter.name.c_str()))
				{
					ConditionMode selectedMode = ConditionMode::Greater;

					if (parameter.value.index() == 2)
						selectedMode = ConditionMode::True;

					m_selectedTransition->AddCondition({ selectedMode, parameter });
				}
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginTable("table1", 3))
		{
			auto& conditions = m_selectedTransition->GetConditions();

			for (int i = 0; i < conditions.size(); i++)
			{
				ImGui::TableNextRow();
				ImGui::PushID(i);

				unsigned int selectedIndex = (unsigned int&)conditions[i].mode;

				ImGui::TableSetColumnIndex(0);

				ImGui::Text(conditions[i].parameter.name.c_str());

				if (conditions[i].parameter.value.index() == 0)
				{
					selectedIndex -= 2;

					ImGui::TableSetColumnIndex(1);
					ImGuiUtils::RenderDropdownList("##mode", { "Greater", "Less" }, selectedIndex);

					ImGui::TableSetColumnIndex(2);
					ImGui::InputFloat("##threshold", &std::get<float>(conditions[i].parameter.value));

					selectedIndex += 2;
				}
				else if (conditions[i].parameter.value.index() == 1)
				{
					selectedIndex -= 2;

					ImGui::TableSetColumnIndex(1);
					ImGuiUtils::RenderDropdownList("##mode", { "Greater", "Less", "Equals", "Not Equal" }, selectedIndex);

					ImGui::TableSetColumnIndex(2);
					ImGui::InputInt("##threshold", &std::get<int>(conditions[i].parameter.value), 0);

					selectedIndex += 2;
				}
				else if (conditions[i].parameter.value.index() == 2)
				{
					ImGui::TableSetColumnIndex(1);
					ImGuiUtils::RenderDropdownList("##mode", { "True", "False" }, selectedIndex);
				}

				conditions[i].mode = (ConditionMode)selectedIndex;

				ImGui::PopID();
			}

			ImGui::EndTable();
		}
	}

}