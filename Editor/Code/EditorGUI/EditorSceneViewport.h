#pragma once

#include <memory>
#include <vector>
#include "../Gizmos/Gizmo.h"
#include "EditorViewport.h"
#include "../EditorData.h"
#include "Scene/Scene.h"

class EditorSceneViewport : public EditorViewport
{
public:
	EditorSceneViewport(EditorLayer& editorLayer);

	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	void RenderTools();

	virtual void OnMouseScrolled(MouseScrolledEvent& event) override;
	virtual void OnWindowResize(WindowResizeEvent& event) override;
	virtual void OnMouseMoved(MouseMovedEvent& event) override;
	virtual void OnMouseButtonPressed(MouseButtonPressedEvent& event) override;
	virtual void OnMouseButtonReleased(MouseButtonReleasedEvent& event) override;

private:
	RenderOptions m_renderOptions;
	std::vector<std::unique_ptr<Gizmo>> m_gizmos;
	GizmoStatus m_gizmoStatus;
};