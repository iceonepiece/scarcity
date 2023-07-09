#pragma once

#include "Core/Application.h"
#include "Gizmos/Gizmo.h"
#include "Core/Camera.h"
#include <vector>
#include <memory>

enum EditorMode
{
	ViewMode,
	TranslateMode,
	RotateMode,
	ScaleMode
};

class Editor2D : public Application
{
public:

	virtual void Initialize(std::string title, int width, int height) override;
	virtual void Run() override;
	virtual void OnEvent(Event* event) override;
	virtual void OnKeyPressed(KeyPressedEvent& event) override;

protected:
	virtual void ProcessInput() override;
	virtual void Update() override;
	virtual void Render() override;

protected:
	EditorMode m_currentMode;
	std::vector<std::unique_ptr<Event>> m_events;
	std::vector<std::unique_ptr<Gizmo>> m_gizmos;
	std::unique_ptr<Camera> m_camera;
};