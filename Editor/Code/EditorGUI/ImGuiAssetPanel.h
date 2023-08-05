#pragma once

#include <filesystem>
#include "Graphics/Texture.h"

class EditorLayer;

class ImGuiAssetPanel
{
public:
	ImGuiAssetPanel(EditorLayer& editor);
	void Render();

	void SetCurrentDirectory(std::filesystem::path path)
	{
		m_CurrentDirectory = path;
	}

private:
	std::filesystem::path m_BaseDirectory;
	std::filesystem::path m_CurrentDirectory;

	std::unique_ptr<Texture> m_folderIcon;
	std::unique_ptr<Texture> m_fileIcon;

	EditorLayer& m_editor;
};