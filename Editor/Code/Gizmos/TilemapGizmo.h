#pragma once

#include "Gizmo.h"

enum class TilemapGizmoMode
{
	None,
	Draw,
	Erase
};

class TilemapGizmo : public Gizmo
{
public:
	TilemapGizmo(EditorViewport& viewport);

	virtual bool OnPicking2D(const glm::vec2& cursorPosition) override;
	virtual void OnMouseButtonPressed(MouseButtonPressedEvent& event) override;

	void SetMode(TilemapGizmoMode mode);

private:
	TilemapGizmoMode m_mode = TilemapGizmoMode::None;
};