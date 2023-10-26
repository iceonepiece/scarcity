#pragma once

#include <filesystem>
#include <IconsFontAwesome6.h>
#include "File/FileSystem.h"
#include "ImGuiWindow.h"
#include "Animations/Sprite.h"

class ImGuiSelectSpriteWindow : public ImGuiWindow_
{
public:
	ImGuiSelectSpriteWindow(EditorLayer& editor, const std::filesystem::path& path);
	virtual void Render() override;

	inline std::filesystem::path GetSelectedPath() { return m_currentDirectory; }
	inline Sprite* GetSelectedSprite() { return m_selectedSprite;  }

private:
	Sprite* m_selectedSprite;
	std::filesystem::path m_baseDirectory;
	std::filesystem::path m_currentDirectory;
};