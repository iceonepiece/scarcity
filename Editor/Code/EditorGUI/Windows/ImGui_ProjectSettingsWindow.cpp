#include "ImGui_ProjectSettingsWindow.h"
#include "Physics/Physics.h"

ImGui_ProjectSettingsWindow::ImGui_ProjectSettingsWindow(EditorLayer& editor)
	: ImGui_Window(editor, "Project Settings")
{
}
void ImGui_ProjectSettingsWindow::Render()
{
	if (!m_isOpen)
		return;

	if (ImGui::Begin(m_windowName.c_str(), &m_isOpen, ImGuiWindowFlags_NoScrollbar))
	{
		auto& collisionLayers = Physics::GetLayers();
		std::vector<uint8_t> availableLayers;

		for (int i = 0; i < collisionLayers.size(); i++)
		{
			if (collisionLayers[i].size() > 0)
				availableLayers.push_back(i);
		}

		if (ImGui::BeginTable("##collisionMatrix", availableLayers.size() + 1, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_BordersInnerV))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("");

			for (int i = availableLayers.size() - 1; i >= 0; i--)
			{
				ImGui::TableNextColumn();
				ImGui::Text(collisionLayers[availableLayers[i]].c_str());
			}

			auto& collisionMatrix = Physics::GetCollisionMatrix();

			for (int i = 0; i < availableLayers.size(); i++)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text(collisionLayers[availableLayers[i]].c_str());

				for (int j = availableLayers.size() - 1; j >= i; j--)
				{
					std::string strID = std::to_string(i) + std::to_string(j);
					ImGui::TableNextColumn();
					ImGui::PushID(strID.c_str());
					bool checked = collisionMatrix[availableLayers[i]][availableLayers[j]];
					ImGui::Checkbox("##layer_name", &checked);
					collisionMatrix[availableLayers[i]][availableLayers[j]] = checked;
					collisionMatrix[availableLayers[j]][availableLayers[i]] = checked;
					ImGui::PopID();
				}
			}

			ImGui::EndTable();
		}
	}

	ImGui::End();
}