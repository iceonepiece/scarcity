#pragma once

#include "ImGui_Window.h"
#include "Components/TilemapComponent.h"

class ImGui_TilemapEditorWindow : public ImGui_Window
{
public:
	ImGui_TilemapEditorWindow(EditorLayer& editor);
	virtual void Render() override;

	void SetTilemap(TilemapComponent* tilemap)
	{
		m_tilemap = tilemap;
	}

	static ImGui_TilemapEditorWindow* Get()
	{
		return s_instance;
	}

private:
	inline static ImGui_TilemapEditorWindow* s_instance = nullptr;

	TilemapComponent* m_tilemap = nullptr;

	std::set<size_t> m_selectedSprites;
};