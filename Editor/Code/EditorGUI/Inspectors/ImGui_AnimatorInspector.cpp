#include "ImGui_AnimatorInspector.h"
#include "../ImGui_AnimatorPanel.h"
#include "Animations/AnimatorState.h"
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
}