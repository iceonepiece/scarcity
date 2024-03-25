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
		m_rows = m_tilemap->rows;
		m_cols = m_tilemap->cols;

	}

	unsigned int GetTileSize() const
	{
		return m_tileSize;
	}

	static ImGui_TilemapEditorWindow* Get()
	{
		return s_instance;
	}

	TilemapComponent* GetTilemap()
	{
		return m_tilemap;
	}

	glm::ivec2 GetStartTile() const
	{
		return { startTile.x, startTile.y };
	}

	glm::ivec2 GetEndTile() const
	{
		return { endTile.x, endTile.y };
	}

private:
	inline static ImGui_TilemapEditorWindow* s_instance = nullptr;

	int m_rows = 1;
	int m_cols = 1;

	inline static std::vector<float> S_ZOOM_VALUES {
		0.25f,	0.33f,	0.50f,	0.67f,	0.75f,	0.80f,	0.90f,	1.00f,
		1.10f,	1.25f,	1.50f,	1.75f,	2.00f,	2.50f,	3.00f,	4.00f,	5.00f
	};

	ImVec2 startTile { -1, -1 };
	ImVec2 endTile { -1, -1 };

	unsigned int m_tileSize = 1;
	TilemapComponent* m_tilemap = nullptr;

	std::unique_ptr<Image> m_tilemapImage = nullptr;

	unsigned int m_zoomIndex = 3;

	std::set<size_t> m_selectedSprites;
};