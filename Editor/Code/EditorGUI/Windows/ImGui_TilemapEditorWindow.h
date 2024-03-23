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
	inline static float S_ZOOM_VALUES[7] = { 0.25f, 0.5f, 0.75f, 1.0f, 1.25f, 1.5f, 2.0f };

	TilemapComponent* m_tilemap = nullptr;

	unsigned int m_zoomIndex = 3;

	std::set<size_t> m_selectedSprites;
};