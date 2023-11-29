#include "ImGui_AnimatorInspector.h"
#include "../ImGui_AnimatorPanel.h"
#include "Animations/AnimatorState.h"
#include "../ImGuiUtils.h"
#include <imgui/imgui_stdlib.h>
#include <IconsFontAwesome6.h>
#include "Core/Application.h"
#include "Asset/AssetManager.h"

void ImGui_AnimatorInspector::Render(AnimatorState& state)
{
	static unsigned int m_selectedMotionIndex = 0;

	ImGui::InputText("Title", &state.m_name);

	auto& animClips = Application::Get().GetAssetManager().GetAnimationClips();

	m_selectedMotionIndex = 0;

	std::vector<std::string> m_availableMotions { "None" };
	for (auto& clip : animClips)
	{
		if (state.m_motion == clip)
			m_selectedMotionIndex = m_availableMotions.size();

		m_availableMotions.push_back(clip->GetName());
	}

	ImGuiUtils::RenderDropdownList("Motion", m_availableMotions, m_selectedMotionIndex);
	state.m_motion = m_selectedMotionIndex == 0 ? nullptr : animClips[m_selectedMotionIndex - 1];

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
		AnimatorController* controller = state.GetController();
		auto& parameters = controller->GetParameters();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 16));
		ImGui::BeginChild("Conditions", ImVec2(0, 0), true);

		ImGui::Text("Conditions");
		ImGui::SameLine();
		if (ImGui::Button("Add"))
		{
			ImGui::OpenPopup("AddConditionPopup");
		}

		if (ImGui::BeginPopup("AddConditionPopup"))
		{
			for (int i = 0; i < parameters.size(); i++)
			{
				AnimatorParameter& parameter = parameters[i];

				if (ImGui::MenuItem(parameter.name.c_str()))
				{
					ConditionMode selectedMode = ConditionMode::Greater;

					if (parameter.value.index() == 2 || parameter.value.index() == 3)
						selectedMode = ConditionMode::True;

					m_selectedTransition->AddCondition({ selectedMode, parameter });
				}
			}

			ImGui::EndPopup();
		}


		int deleteIndex = -1;

		if (ImGui::BeginTable("table1", 3))
		{
			auto& conditions = m_selectedTransition->GetConditions();

			for (int i = 0; i < conditions.size(); i++)
			{
				ImGui::TableNextRow();
				ImGui::PushID(i);

				unsigned int selectedIndex = (unsigned int&)conditions[i].mode;

				ImGui::TableSetColumnIndex(0);

				if (ImGui::Button(ICON_FA_MINUS))
					deleteIndex = i;

				ImGui::SameLine();
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
					conditions[i].parameter.value = selectedIndex == 0 ? true : false;
				}

				conditions[i].mode = (ConditionMode)selectedIndex;

				ImGui::PopID();
			}

			ImGui::EndTable();
		}

		if (deleteIndex != -1)
			m_selectedTransition->RemoveCondition(deleteIndex);

		ImGui::EndChild();
		ImGui::PopStyleVar();
	}
}