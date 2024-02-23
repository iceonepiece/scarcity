#include "ImGui_ProjectSettingsWindow.h"
#include "Physics/Physics.h"
#include "Project/Project.h"
#include <ImGui/imgui_stdlib.h>

ImGui_ProjectSettingsWindow::ImGui_ProjectSettingsWindow(EditorLayer& editor)
	: ImGui_Window(editor, "Project Settings")
{
}

void ImGui_ProjectSettingsWindow::RenderPhysics()
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

void ImGui_ProjectSettingsWindow::RenderLuaScripts()
{
	static std::string luaScriptPath;

	Project* project = Project::GetActive();
	auto& luaScripts = project->GetLuaScripts();

	ImGui::InputText("##lua_script_path", &luaScriptPath);
	ImGui::SameLine();

	if (ImGui::Button("Add Lua Script"))
	{
		std::filesystem::path absoluteScriptPath = project->GetAbsolutePath() / luaScriptPath;
		if (Asset* asset = project->GetAssetManager().GetAsset(absoluteScriptPath))
		{
			luaScripts.push_back(dynamic_cast<LuaScript*>(asset));
		}

	}

	ImGui::Text("List of Lua Scripts");
	ImGui::Separator();

	int toDeleteIndex = -1;

	for (int i = 0; i < luaScripts.size(); i++)
	{
		ImGui::PushID(i);
		
		if (ImGui::Button("Delete"))
			toDeleteIndex = i;

		ImGui::SameLine();
		ImGui::Text(luaScripts[i]->GetName().c_str());

		ImGui::PopID();
	}

	if (toDeleteIndex != -1)
		luaScripts.erase(luaScripts.begin() + toDeleteIndex);
}

void ImGui_ProjectSettingsWindow::Render()
{
	if (!m_isOpen)
		return;

	if (ImGui::Begin(m_windowName.c_str(), &m_isOpen, ImGuiWindowFlags_NoScrollbar))
	{
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("project_settings", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Physics"))
			{
				RenderPhysics();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Lua Scripts"))
			{
				RenderLuaScripts();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}