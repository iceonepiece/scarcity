#include "imgui/imgui.h"
#include <IconsFontAwesome6.h>
#include "ImGuiAssetPanel.h"
#include <string>
#include "Platforms/OpenGL/OpenGLTexture.h"
#include "../EditorLayer.h"

ImGuiAssetPanel::ImGuiAssetPanel(EditorLayer& editor)
	: m_editor(editor)
	, m_BaseDirectory(std::filesystem::current_path())
	, m_CurrentDirectory(m_BaseDirectory)
{
	m_folderIcon = std::make_unique<OpenGLTexture>("Editor/icons8-folder-200.png");
	m_fileIcon = std::make_unique<OpenGLTexture>("Editor/icons8-file-200.png");
}

void ImGuiAssetPanel::Render()
{
	ImGui::Begin("Project");

	if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
	{
		if (ImGui::Button("<-"))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}
	}

	static float padding = 16.0f;
	static float thumbnailSize = 128.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		const auto& path = directoryEntry.path();

		if (path.extension() == ".meta")
			continue;

		std::string filenameString = path.filename().string();

		ImGui::PushID(filenameString.c_str());
		Texture* icon = directoryEntry.is_directory() ? m_folderIcon.get() : m_fileIcon.get();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		//ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

		std::string useIcon = directoryEntry.is_directory() ? (ICON_FA_FOLDER " ") : (ICON_FA_FILE " ");

		if (ImGui::Selectable(std::string(useIcon + filenameString).c_str(), m_editor.GetSelectedPath() == path))
		{
			m_editor.SetSelectedPath(path);
		}

		//ImGui::Button("SSS", { thumbnailSize, thumbnailSize });

		if (ImGui::BeginDragDropSource())
		{
			std::filesystem::path relativePath(path);
			const wchar_t* itemPath = relativePath.c_str();
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}

		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
				m_CurrentDirectory /= path.filename();
			else if (path.extension() == SCENE_FILE_EXTENSION)
			{
				std::cout << "Clicked file with extension: " << path.extension() << std::endl;
				m_editor.OpenScene(path);
			}
			else
			{
				std::string command = "\"" + path.string() + "\"";
				std::system(command.c_str());
			}


		}
		//ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}

	ImGui::Columns(1);

	ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
	ImGui::SliderFloat("Padding", &padding, 0, 32);

	// TODO: status bar
	ImGui::End();
}