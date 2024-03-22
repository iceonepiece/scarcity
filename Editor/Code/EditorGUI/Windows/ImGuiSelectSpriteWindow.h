#pragma once

#include <filesystem>
#include <IconsFontAwesome6.h>
#include "File/FileSystem.h"
#include "ImGui_Window.h"
#include "Graphics/Sprite.h"

class ImGuiSelectSpriteWindow : public ImGui_Window
{
public:
	ImGuiSelectSpriteWindow(EditorLayer& editor, const std::filesystem::path& path);
	virtual void Render() override;

	inline std::filesystem::path GetSelectedPath() { return m_currentDirectory; }
	inline Sprite* GetSelectedSprite() { return m_selectedSprite; }
	inline size_t GetSpriteIndex() { return m_spriteIndex; }

	static ImGuiSelectSpriteWindow* Get()
	{
		return s_instance;
	}

private:
	inline static ImGuiSelectSpriteWindow* s_instance = nullptr;

	Sprite* m_selectedSprite;
	size_t m_spriteIndex;
	std::filesystem::path m_baseDirectory;
	std::filesystem::path m_currentDirectory;
};