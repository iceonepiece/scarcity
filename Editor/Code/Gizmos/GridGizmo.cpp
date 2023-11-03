#include "GridGizmo.h"
#include "Components/GridComponent.h"
#include "../EditorLayer.h"

GridGizmo::GridGizmo(EditorViewport& viewport)
	: Gizmo(viewport)
{}

bool GridGizmo::OnPicking2D(const glm::vec2& cursorPosition)
{
	EditorLayer& editor = m_viewport.GetEditorLayer();

	if (GridComponent* grid = editor.GetSelectedEntity().GetComponent<GridComponent>())
	{
		int x = (int)std::floor(cursorPosition.x / grid->size);
		int y = (int)std::floor(cursorPosition.y / grid->size);

		auto pair = std::make_pair(x, y);

		if (m_mode == GridGizmoMode::Draw)
			grid->cellMap[pair] = 1;
		else if (m_mode == GridGizmoMode::Erase)
		{
			if (grid->cellMap.find(pair) != grid->cellMap.end())
				grid->cellMap.erase(pair);
		}
	}

	return true;
}

void GridGizmo::SetMode(GridGizmoMode mode)
{
	m_mode = mode;
}

void GridGizmo::OnMouseButtonPressed(MouseButtonPressedEvent& event)
{
}