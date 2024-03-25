#pragma once

enum EditorMode
{
	ViewMode,
	TranslateMode,
	RotateMode,
	ScaleMode,
	GridMode,
	TilemapMode,
};

enum class EditorObjectType
{
	None,
	Entity,
	Asset,
	AnimatorState
};

struct GizmoStatus
{
	bool dragging = false;
	EditorMode mode = ViewMode;
};
