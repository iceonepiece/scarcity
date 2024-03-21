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

private:
	TilemapComponent* m_tilemap = nullptr;

	std::set<size_t> m_selectedSprites;
};