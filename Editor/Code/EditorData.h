#pragma once

enum EditorMode
{
	ViewMode,
	TranslateMode,
	RotateMode,
	ScaleMode,
	GridMode
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
