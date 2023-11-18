#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include "EditorData.h"
#include "Core/Layer.h"
#include "Scene/Scene.h"
#include "EditorApplication.h"
#include "Gizmos/Gizmo.h"
#include "NativeScript/NativeScriptEngine.h"
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
#include "EditorGUI/EditorSceneViewport.h"
#include "EditorGUI/ImGui_AnimatorPanel.h"
#include "EditorGUI/ImGui_InspectorPanel.h"
#include "GameLayer.h"
#include "Commands/EditorCommand.h"
#include "Graphics/Framebuffer.h"


struct EditorObject
{
	EditorObjectType type = EditorObjectType::None;
	entt::entity entity = entt::null;
	//Asset* asset = nullptr;
	std::string note = "";
	void* objectPtr = nullptr;
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
		if (m_selectedObject.type == EditorObjectType::Asset)
		{
			if (Asset* asset = reinterpret_cast<Asset*>(m_selectedObject.objectPtr))
				return asset->GetPath();
		}

		return "";
	}

	EditorObject& SetSelectedObject(EditorObjectType type, void* objectPtr);
	void SetSelectedAsset(Asset* asset, const std::string& note = "");
	void SetSelectedPath(const std::filesystem::path& path, const std::string& note = "");

	inline bool IsGridModeAvailable() { return m_gridModeAvailable; }

	void SetPickedEntity(entt::entity picked);
	void UnselectObject();

	void DuplicateEntity();
	void DeleteEntity(entt::entity entity);

	bool NewScene();
	bool OpenScene(std::filesystem::path absolutePath);
	bool SaveScene();
	bool SaveSceneAs();

	bool SaveProject();

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

	bool CheckPicking2D(const glm::vec2& cursorPosition);

	inline EditorObject& GetSelectedObject()
	{
		return m_selectedObject;
	}

	inline Asset* GetSelectedAsset()
	{
		return reinterpret_cast<Asset*>(m_selectedObject.objectPtr);
	}

	static Asset* GetAsset(const std::filesystem::path& path);

	static ImGuiWindow_* GetImGuiWindow(ImGuiWindowType windowType)
	{
		if (s_instance->m_imGuiWindowMap.find(windowType) != s_instance->m_imGuiWindowMap.end())
			return s_instance->m_imGuiWindowMap.at(windowType).get();

		return nullptr;
	}

	inline void SetMouseActive(bool active)
	{
		m_mouseActive = active;
	}

	inline bool IsMouseActive()
	{
		return m_mouseActive;
	}

	inline EditorMode GetCurrentMode()
	{
		return m_currentMode;
	}

	inline void SetCurrentMode(EditorMode mode)
	{
		m_currentMode = mode;
	}

	inline GameLayer& GetGameLayer()
	{
		return m_gameLayer;
	}

	void SetAnimatorController(AnimatorController& animController)
	{
		m_animatorPanel.SetAnimatorController(animController);
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
	std::unique_ptr<Framebuffer> m_sceneFramebuffer;

	glm::vec2 m_viewportSize = { 0.0f, 0.0f };
	bool m_viewportFocused = false;
	bool m_viewportHovered = false;

	GizmoStatus m_gizmoStatus;
	std::vector<std::unique_ptr<EditorCommand>> m_editorCommands;
	int m_currentCommandIndex = -1;
	bool m_gridModeAvailable = false;

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
	glm::vec2 m_viewportCursorPosition;

	ImGuiMainMenuBar m_mainMenuBar;
	ImGuiHierarchy m_hierarchy;
	ImGuiAssetPanel m_assetPanel;
	ImGui_AnimatorPanel m_animatorPanel;
	ImGui_InspectorPanel m_inspectorPanel;

	EditorSceneViewport m_editorSceneViewport;

	std::unordered_map<ImGuiWindowType, std::unique_ptr<ImGuiWindow_>> m_imGuiWindowMap;
};