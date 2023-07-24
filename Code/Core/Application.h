#pragma once

#include <string>
#include <memory>
#include "Window.h"
#include "ScriptingEngine.h"
#include "Graphics/Renderer.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"
#include "Lua/LuaEngine.h"

class Application
{
public:
	virtual ~Application() = default;

	virtual void Initialize(std::string title, int width, int height) = 0;
	virtual void Run() = 0;

	template <typename T>
	void OnEvent(T& e) {}

	virtual void OnEvent(Event* e) {}


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
	inline ScriptingEngine* GetScriptingEngine() { return &m_scriptingEngine; }
	inline LuaEngine& GetLuaEngine() { return m_luaEngine; }
	inline Renderer& GetRenderer() { return *m_renderer; }
	inline Window& GetWindow() { return *m_window; }

protected:
	virtual void ProcessInput() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	bool m_running = true;
	std::unique_ptr<Window> m_window;
	std::unique_ptr<Renderer> m_renderer;

	LuaEngine m_luaEngine;
	ScriptingEngine m_scriptingEngine;
};