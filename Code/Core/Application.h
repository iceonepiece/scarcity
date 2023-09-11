#pragma once

#include "PCH.h"
#include "Window.h"
//#include "Graphics/Renderer.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"
#include "Input/NewInput.h"

class LuaEngine;
class ImGuiManager;
class Layer;
class Renderer;

struct ApplicationConfigs
{
	std::string name = "BossFight Application";
};

class Application
{
public:
	Application(const ApplicationConfigs& configs);
	virtual ~Application();

	static Application& Get() { return *s_instance; }

	virtual void Initialize(std::string title, int width, int height) = 0;
	void Run();

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
	inline LuaEngine& GetLuaEngine() { return *m_luaEngine; }
	inline Renderer& GetRenderer() { return *m_renderer; }
	inline Window& GetWindow() { return *m_window; }
	inline NewInput& GetInput() { return *m_input; }

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
	std::unique_ptr<LuaEngine> m_luaEngine;
	std::unique_ptr<NewInput> m_input;

	std::vector<std::unique_ptr<Layer>> m_layers;
};