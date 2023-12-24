#pragma once

#include <array>
#include <map>
#include "PCH.h"
#include "Window.h"
//#include "Graphics/Renderer.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"
#include "Input/Input.h"
#include "Scene/Scene.h"
#include "Entity/EntityManager.h"
#include "Asset/AssetManager.h"
#include "Core/TagManager.h"

class NativeScriptEngine;
class LuaEngine;
class ImGuiManager;
class Layer;
class Renderer;

constexpr int TAG_SIZE = 8;

struct ApplicationConfigs
{
	std::string name = "BossFight Application";
	uint32_t width = 1280;
	uint32_t height = 720;
};

class Application
{
public:
	Application(const ApplicationConfigs& configs);
	virtual ~Application();

	static Application& Get() { return *s_instance; }

	virtual void Initialize() = 0;
	void Run();
	void Close();

	virtual void OnEvent(Event& e);

	void AddLayer(std::unique_ptr<Layer> layer);
	void PopLayer();

	virtual void OnWindowClose() {}
	virtual void OnWindowResize(int width, int height) {}
	
	virtual void OnMouseButtonPressed(int button) {}
	virtual void OnMouseButtonReleased(int button) {}

	virtual void OnMouseButtonEvent(MouseButtonEvent& event) {}
	virtual void OnKeyEvent(KeyEvent& event) {}

	virtual void OnKeyPressed(KeyPressedEvent& event) {}
	virtual void OnMouseMoved(float x, float y) {}
	virtual void OnMouseScroll(float x, float y) {}

	virtual void ChangeScene(std::string name) {}
	inline NativeScriptEngine& GetNativeScriptEngine() { return *m_nativeScriptEngine; }
	inline LuaEngine& GetLuaEngine() { return *m_luaEngine; }
	inline Renderer& GetRenderer() { return *m_renderer; }
	inline Window& GetWindow() { return *m_window; }
	inline Input& GetInput() { return *m_input; }
	inline AssetManager& GetAssetManager() { return *m_assetManager; }
	inline EntityManager& GetPrefabManager() { return m_prefabManager; }

	void AddPrefab(Entity entity);
	Entity GetPrefabByName(const std::string& name);

protected:
	virtual void ProcessInput() {}
	virtual void Update() {}
	virtual void Render() {}

	static Application* s_instance;
	ApplicationConfigs m_configs;

	bool m_running = true;
	std::unique_ptr<Window> m_window;
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<ImGuiManager> m_imguiManager;
	std::unique_ptr<NativeScriptEngine> m_nativeScriptEngine;
	std::unique_ptr<LuaEngine> m_luaEngine;
	std::unique_ptr<Input> m_input;
	std::unique_ptr<AssetManager> m_assetManager;

	EntityManager m_prefabManager;

	std::unordered_map<std::string, Entity> m_prefabMap;

	std::vector<std::unique_ptr<Layer>> m_layers;
};