#pragma once

#include <memory>
#include <string>
#include "Window.h"
#include "ScriptingEngine.h"

class Application
{
public:
	virtual ~Application() = default;

	virtual void Initialize(std::string title, int width, int height) = 0;
	virtual void Run() = 0;

	virtual void ChangeScene(std::string name) {}

protected:
	virtual void ProcessInput() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	bool m_running = true;
	std::unique_ptr<Window> m_window;

	ScriptingEngine m_scriptingEngine;
};