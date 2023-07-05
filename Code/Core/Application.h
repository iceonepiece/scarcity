#pragma once

#include <string>
#include "Common.h"
#include "Window.h"
#include "ScriptingEngine.h"
#include "Graphics/Renderer.h"

class Application
{
public:
	virtual ~Application() = default;

	virtual void Initialize(std::string title, int width, int height) = 0;
	virtual void Run() = 0;

	virtual void ChangeScene(std::string name) {}
	inline ScriptingEngine* GetScriptingEngine() { return &m_scriptingEngine; }
	inline Renderer& GetRenderer() { return *m_renderer; }

protected:
	virtual void ProcessInput() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	bool m_running = true;
	Unique<Window> m_window;
	Unique<Renderer> m_renderer;

	ScriptingEngine m_scriptingEngine;
};