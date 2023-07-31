#pragma once

#include <filesystem>
#include "Graphics/Texture.h"

class ImGuiAssetPanel
{
public:
	ImGuiAssetPanel();
	void Render();

private:
	std::filesystem::path m_BaseDirectory;
	std::filesystem::path m_CurrentDirectory;

	std::unique_ptr<Texture> m_folderIcon;
	std::unique_ptr<Texture> m_fileIcon;
};