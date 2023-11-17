#include "ImGui_AnimatorInspector.h"
#include "../ImGui_AnimatorPanel.h"
#include <imgui/imgui_stdlib.h>

void ImGui_AnimatorInspector::Render(Node& node)
{
	ImGui::InputText("Title", &(node.title));

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

	ImGui::InputFloat("Speed", &node.speed);
}