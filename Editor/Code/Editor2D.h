#pragma once

#include "Core/Application.h"
#include "Core/Camera.h"
#include <vector>
#include <memory>
#include "Core/EntityManager.h"
#include "Core/Entity.h"
#include "Gizmos/Gizmo.h"
#include "Components/TransformComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneSerializer.h"

enum EditorMode
{
	ViewMode,
	TranslateMode,
	RotateMode,
	ScaleMode
};

enum class ButtonState
{
	None,
	Pressed,
	Held,
	Released
};

class Editor2D : public Application
{
public:

	struct MouseData
	{
		ButtonState left = ButtonState::None;
		ButtonState right = ButtonState::None;
	};

	virtual void Initialize(std::string title, int width, int height) override;
	virtual void Run() override;
	virtual void OnEvent(Event* event) override;
	virtual void OnKeyPressed(KeyPressedEvent& event) override;
	virtual void OnMouseButtonPressed(int button) override;
	virtual void OnMouseButtonReleased(int button) override;
	virtual void OnMouseMoved(float x, float y) override;
	virtual void OnMouseScroll(float x, float y) override;
	virtual void OnWindowResize(int width, int height) override;
	virtual void OnWindowClose() override;

	TransformComponent* GetEntityTransform();

protected:
	virtual void ProcessInput() override;
	virtual void Update() override;
	virtual void Render() override;

	void OpenScene();
	void SaveScene();

	bool CheckPicking2D();
	void CalculateWorldCursorPosition();

protected:
	std::unique_ptr<Scene> m_scene;

	bool m_mouseActive = false;
	MouseData m_mouseData;

	EditorMode m_currentMode;

	glm::vec2 m_cursorPosition;
	glm::vec2 m_worldCursorPosition;
	entt::entity m_pickedEntity;
	bool m_entityPicked;
	//Entity m_pickedEntity;

	std::vector<std::unique_ptr<Event>> m_events;
	std::vector<std::unique_ptr<Gizmo>> m_gizmos;
	std::unique_ptr<Camera> m_camera;
};