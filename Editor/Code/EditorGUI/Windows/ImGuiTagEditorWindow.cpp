#include "ImGuiTagEditorWindow.h"
#include "Core/Application.h"
#include <IconsFontAwesome6.h>
#include "imgui/imgui_stdlib.h"

ImGuiTagEditorWindow::ImGuiTagEditorWindow(EditorLayer& editor)
	: ImGui_Window(editor)
{

}

void ImGuiTagEditorWindow::Render()
{
	static std::string newTagName = "New tag";
	static int selectedIndex = -1;

	auto& tagManager = Application::Get().GetTagManager();
	auto& tags = tagManager.GetTags();

	if (ImGui::TreeNodeEx("Tags", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool isEmpty = tags.size() == 0;

		if (ImGui::BeginTable("##tags", isEmpty ? 1 : 2, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_BordersInnerV))
		{
			if (isEmpty)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("List is Empty");
			}
			else
			{
				ImGui::TableSetupColumn("one", ImGuiTableColumnFlags_WidthFixed, 100.0f);

				for (int i = 0; i < tags.size(); i++)
				{
					std::string num = "Tag " + std::to_string(i);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text(num.c_str());
					ImGui::TableNextColumn();

					ImGui::PushID(tags[i].c_str());

					if (ImGui::Selectable(tags[i].c_str(), selectedIndex == i, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_DontClosePopups))
						selectedIndex = i;

					ImGui::PopID();
				}
			}

			ImGui::EndTable();
		}

		ImGui::PushItemWidth(120);
		if (ImGui::Button(ICON_FA_PLUS))
		{
			ImGui::OpenPopup("new_tag");
		}

		if (ImGui::BeginPopup("new_tag"))
		{
			ImGui::Text("New Tag Name");
			ImGui::SameLine();
			ImGui::InputText("##new_tag_name", &newTagName);

			if (ImGui::Button("Save"))
			{
				tagManager.AddTag(newTagName);
				ImGui::CloseCurrentPopup();

				newTagName = "New tag";
			}


			ImGui::EndPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_MINUS))
		{
			if (selectedIndex >= 0)
				tagManager.RemoveTag(selectedIndex);

			selectedIndex = -1;
		}

		ImGui::PopItemWidth();

		ImGui::TreePop();
	}
}