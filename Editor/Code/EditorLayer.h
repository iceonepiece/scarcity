#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include "Core/Layer.h"
#include "Scene/Scene.h"
#include "EditorApplication.h"
#include "Gizmos/Gizmo.h"
#include "NativeScript/NativeScriptEngine.h"
#include "EditorGUI/ImGuiNodeEditor.h"
#include "File/FileSystem.h"
#include <filewatch/FileWatch.h>
#include "Helpers/FileHandler.h"
#include "Asset/Asset.h"
#include "Animations/Sprite.h"
#include "EditorGUI/Windows/ImGuiWindow.h"
#include "EditorGUI/ImGuiMainMenuBar.h"
#include "EditorGUI/ImGuiEntityProperties.h"
#include "EditorGUI/ImGuiHierarchy.h"
#include "EditorGUI/ImGuiAssetPanel.h"
#include "GameLayer.h"
#include "Commands/EditorCommand.h"


enum EditorMode
{
	ViewMode,
	TranslateMode,
	RotateMode,
	ScaleMode
};

enum class EditorObjectType
{
	None,
	Entity,
	Path
};

struct GizmoStatus
{
	bool dragging = false;
	EditorMode mode = ViewMode;
};

struct EditorObject
{
	EditorObjectType type = EditorObjectType::None;
	entt::entity entity = entt::null;
	Asset* asset = nullptr;
	std::string note = "";
};

class EditorLayer : public Layer
{
public:
	EditorLayer(EditorApplication& app, std::unique_ptr<Project> project);
	virtual ~EditorLayer() = default;

	virtual void Update(float deltaTime) override;
	virtual void OnEvent(Event& event) override;
	virtual void Initialize() override;
	virtual void Shutdown() override;
	virtual void RenderImGui() override;

	void AddCommand(EditorCommand* command);
	void RedoCommand();
	void UndoCommand();

	void OnSceneUpdate();

	void CreatePrefab(entt::entity entity, const std::filesystem::path& path);

	inline bool IsEntityPicked() { return m_entityPicked; }

	Entity GetSelectedEntity();

	inline entt::entity GetPickedEntity()
	{
		return m_selectedObject.entity;
	}

	inline std::filesystem::path GetSelectedPath()
	{
		if (m_selectedObject.type == EditorObjectType::Path && m_selectedObject.asset)
			return m_selectedObject.asset->GetPath();

		return "";
	}

	void SetSelectedAsset(Asset* asset, const std::string& note);
	void SetSelectedPath(const std::filesystem::path& path, const std::string& note = "");
	void LoadAsset(const std::filesystem::path& path);

	void SetPickedEntity(entt::entity picked);
	void UnselectObject();

	void DuplicateEntity();
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
	void ChangeScene(const std::string& name)
	{
		m_gameLayer.ChangeScene(name);
	}

	Camera& GetCamera() { return *m_camera; }

	bool CheckPicking2D();
	void CalculateWorldCursorPosition();

	inline EditorObject& GetSelectedObject() { return m_selectedObject; }
	inline Asset* GetSelectedAsset() { return m_selectedObject.asset; }

	static Asset* GetAsset(const std::filesystem::path& path);

	static ImGuiWindow* GetImGuiWindow(ImGuiWindowType windowType)
	{
		if (s_instance->m_imGuiWindowMap.find(windowType) != s_instance->m_imGuiWindowMap.end())
			return s_instance->m_imGuiWindowMap.at(windowType).get();

		return nullptr;
	}

private:
	void OnWindowResize(WindowResizeEvent& event);
	void OnMouseMoved(MouseMovedEvent& event);
	void OnMouseButtonPressed(MouseButtonPressedEvent& event);
	void OnMouseButtonReleased(MouseButtonReleasedEvent& event);
	void OnKeyPressed(KeyPressedEvent& event);
	void OnMouseScrolled(MouseScrolledEvent& event);

	void OnFileEvent(const FileEvent& event);

private:
	GizmoStatus m_gizmoStatus;
	std::vector<std::unique_ptr<EditorCommand>> m_editorCommands;
	int m_currentCommandIndex = -1;

	GameLayer m_gameLayer;
	static EditorLayer* s_instance;

	EntityManager m_prefabManager;
	std::unordered_map<std::string, Sprite> m_spriteMap;

	std::unordered_map<std::string, std::unique_ptr<Asset>> m_assetMap;

	std::mutex m_fileEventMutex;
	std::vector<FileEvent> m_fileEvents;
	std::unique_ptr<filewatch::FileWatch<std::string>> m_fileWatcher;
	FileHandler m_fileHandler;

	EditorObject m_selectedObject;

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

	EditorMode m_currentMode;

	glm::vec2 m_cursorPosition;
	glm::vec2 m_worldCursorPosition;

	ImGuiMainMenuBar m_mainMenuBar;
	ImGuiEntityProperties m_entityProperties;
	ImGuiHierarchy m_hierarchy;
	ImGuiAssetPanel m_assetPanel;
	ImGuiNodeEditor m_nodeEditor;

	std::unordered_map<ImGuiWindowType, std::unique_ptr<ImGuiWindow>> m_imGuiWindowMap;
};