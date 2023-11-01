#pragma once

#include "Gizmo.h"

enum class GridGizmoMode
{
	None,
	Draw,
	Erase
};

class GridGizmo : public Gizmo
{
public:
	GridGizmo(EditorViewport& viewport);

	virtual bool OnPicking2D(const glm::vec2& cursorPosition) override;
	virtual void OnMouseButtonPressed(MouseButtonPressedEvent& event) override;

	void SetMode(GridGizmoMode mode);

private:
	GridGizmoMode m_mode = GridGizmoMode::None;
};