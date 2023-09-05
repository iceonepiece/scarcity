#pragma once

#include <filesystem>
#include "Graphics/Texture.h"
#include "Asset/TextureAsset.h"

class EditorLayer;

class ImGuiAssetPanel
{
public:
	ImGuiAssetPanel(EditorLayer& editor);
	void Render();

	void SetProjectDirectory(std::filesystem::path path)
	{
		m_CurrentDirectory = path;
	}

	void RenderTexture(TextureAsset& textureAsset, ImGuiTreeNodeFlags flags);
	void RenderFolder(const std::filesystem::path& path, ImGuiTreeNodeFlags flags);
	void RenderUnsupportedFile(const std::filesystem::path& path);

private:
	std::filesystem::path m_selectedPath;
	std::filesystem::path m_BaseDirectory;
	std::filesystem::path m_CurrentDirectory;

	std::unique_ptr<Texture> m_folderIcon;
	std::unique_ptr<Texture> m_fileIcon;

	EditorLayer& m_editor;
};