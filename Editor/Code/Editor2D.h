#pragma once

#include "Core/Application.h"
#include "Core/Camera.h"
#include <vector>
#include <memory>
#include "Core/EntityManager.h"
#include "Core/Entity.h"
#include "Gizmos/Gizmo.h"
#include "Components/TransformComponent.h"

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
	virtual void OnMouseButtonPressed(MouseButtonPressedEvent& event) override;
	virtual void OnMouseMoved(MouseMovedEvent& event) override;
	virtual void OnWindowResize(WindowResizeEvent& event) override;

	TransformComponent* GetEntityTransform();

protected:
	virtual void ProcessInput() override;
	virtual void Update() override;
	virtual void Render() override;

	bool CheckPicking2D();
	void CalculateWorldCursorPosition();

protected:
	EditorMode m_currentMode;

	glm::vec2 m_cursorPosition;
	glm::vec2 m_worldCursorPosition;
	EntityManager m_entityManager;
	entt::entity m_pickedEntity;
	bool m_entityPicked;
	//Entity m_pickedEntity;

	std::vector<std::unique_ptr<Event>> m_events;
	std::vector<std::unique_ptr<Gizmo>> m_gizmos;
	std::unique_ptr<Camera> m_camera;
};