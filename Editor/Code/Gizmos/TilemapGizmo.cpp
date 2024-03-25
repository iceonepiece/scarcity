#include "TilemapGizmo.h"
#include "Components/TilemapComponent.h"
#include "../EditorLayer.h"
#include "../EditorGUI/Windows/ImGui_TilemapEditorWindow.h"

TilemapGizmo::TilemapGizmo(EditorViewport& viewport)
	: Gizmo(viewport)
{}

bool TilemapGizmo::OnPicking2D(const glm::vec2& cursorPosition)
{
	EditorLayer& editor = m_viewport.GetEditorLayer();

	if (TilemapComponent* tilemap = editor.GetSelectedEntity().GetComponent<TilemapComponent>())
	{
		int x = (int)std::floor(cursorPosition.x / 1.0f);
		int y = (int)std::floor(cursorPosition.y / 1.0f);

		auto pair = std::make_pair(x, y);

		if (m_mode == TilemapGizmoMode::Draw)
		{
			auto window = ImGui_TilemapEditorWindow::Get();
			glm::ivec2 startTile = window->GetStartTile();
			glm::ivec2 endTile = window->GetEndTile();

			unsigned int numRows = endTile.y - startTile.y + 1;
			unsigned int numCols = endTile.x - startTile.x + 1;

			for (int i = 0; i < numRows; i++)
			{
				for (int j = 0; j < numCols; j++)
				{
					auto worldPair = std::make_pair(pair.first + j, pair.second - i);
					auto texturePair = std::make_pair(startTile.y + i, startTile.x + j);

					tilemap->data[worldPair] = texturePair;
				}
			}
			std::cout << std::endl;
		}
		else if (m_mode == TilemapGizmoMode::Erase)
		{
			if (tilemap->data.find(pair) != tilemap->data.end())
				tilemap->data.erase(pair);
		}
		else
			return false;
	}

	return true;
}

void TilemapGizmo::SetMode(TilemapGizmoMode mode)
{
	m_mode = mode;
}

void TilemapGizmo::OnMouseButtonPressed(MouseButtonPressedEvent& event)
{
}