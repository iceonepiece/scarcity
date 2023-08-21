#pragma once

#include <unordered_map>
#include "Core/Layer.h"
#include "EditorGUI/ImGuiMain.h"
#include "Scene/Scene.h"
#include "EditorApplication.h"
#include "Gizmos/Gizmo.h"
#include "NativeScript/NativeScriptEngine.h"

enum EditorMode
{
	ViewMode,
	TranslateMode,
	RotateMode,
	ScaleMode
};

class EditorLayer : public Layer
{
public:
	EditorLayer(EditorApplication& app, std::unique_ptr<Project> project);
	EditorLayer(EditorApplication& app, std::unique_ptr<Scene> scene);
	virtual ~EditorLayer() = default;

	virtual void Update(float deltaTime) override;
	virtual void OnEvent(Event& event) override;
	virtual void Initialize() override;
	virtual void Shutdown() override;
	virtual void RenderImGui() override;

	void OnSceneUpdate();

	inline bool IsEntityPicked() { return m_entityPicked; }

	inline entt::entity GetPickedEntity() { return m_pickedEntity; }
	inline void SetPickedEntity(entt::entity picked)
	{
		m_entityPicked = true;
		m_pickedEntity = picked;
	}

	void DeleteEntity(entt::entity entity);

	bool NewScene();
	bool OpenScene(std::filesystem::path path);
	bool SaveScene();
	bool SaveSceneAs();

	void PlayScene();
	void StopScene();
	void ReloadNativeScripts();

	TransformComponent* GetEntityTransform();

	inline bool IsScenePlaying() { return m_scenePlaying; }

	Scene* GetScene() { return m_activeScene.get(); }
	Camera& GetCamera() { return *m_camera; }

	bool CheckPicking2D();
	void CalculateWorldCursorPosition();


private:
	void OnWindowResize(WindowResizeEvent& event);
	void OnMouseMoved(MouseMovedEvent& event);
	void OnMouseButtonPressed(MouseButtonPressedEvent& event);
	void OnMouseButtonReleased(MouseButtonReleasedEvent& event);
	void OnKeyPressed(KeyPressedEvent& event);
	void OnMouseScrolled(MouseScrolledEvent& event);

private:
	NativeScriptEngine m_nativeScriptEngine;
	std::vector<std::string> m_nativeClassNames;

	std::unique_ptr<Project> m_activeProject;
	std::unique_ptr<Scene> m_activeScene;

	bool m_mouseActive = false;
	std::vector<std::unique_ptr<Gizmo>> m_gizmos;
	
	EditorApplication& m_app;

	entt::entity m_pickedEntity;
	bool m_entityPicked;
	bool m_scenePlaying = false;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<ImGuiMain> m_imgui;

	std::unique_ptr<Scene> m_playingScene = nullptr;

	EditorMode m_currentMode;

	glm::vec2 m_cursorPosition;
	glm::vec2 m_worldCursorPosition;

	ImGuiMainMenuBar m_mainMenuBar;
	ImGuiEntityProperties m_entityProperties;
	ImGuiHierarchy m_hierarchy;
	ImGuiAssetPanel m_assetPanel;
};