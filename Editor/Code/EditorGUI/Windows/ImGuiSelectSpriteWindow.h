#pragma once

#include <filesystem>
#include <IconsFontAwesome6.h>
#include "File/FileSystem.h"
#include "ImGuiWindow.h"
#include "Graphics/Sprite.h"

class ImGuiSelectSpriteWindow : public ImGuiWindow_
{
public:
	ImGuiSelectSpriteWindow(EditorLayer& editor, const std::filesystem::path& path);
	virtual void Render() override;

	inline std::filesystem::path GetSelectedPath() { return m_currentDirectory; }
	inline Sprite* GetSelectedSprite() { return m_selectedSprite; }
	inline size_t GetSpriteIndex() { return m_spriteIndex; }

private:
	Sprite* m_selectedSprite;
	size_t m_spriteIndex;
	std::filesystem::path m_baseDirectory;
	std::filesystem::path m_currentDirectory;
};