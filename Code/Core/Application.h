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

class Application
{
public:
	virtual ~Application() = default;

	virtual void Initialize(std::string title, int width, int height) = 0;
	virtual void Run() = 0;

	virtual void OnEvent(Event* e) {}
	virtual void OnKeyPressed(KeyPressedEvent& event) {}
	virtual void OnMouseButtonPressed(MouseButtonPressedEvent& event) {}
	virtual void OnMouseMoved(MouseMovedEvent& event) {}
	virtual void OnWindowResize(WindowResizeEvent& event) {}

	virtual void ChangeScene(std::string name) {}
	inline ScriptingEngine* GetScriptingEngine() { return &m_scriptingEngine; }
	inline Renderer& GetRenderer() { return *m_renderer; }

protected:
	virtual void ProcessInput() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	bool m_running = true;
	std::unique_ptr<Window> m_window;
	std::unique_ptr<Renderer> m_renderer;

	ScriptingEngine m_scriptingEngine;
};