#pragma once

#include "Window.h"

class Application
{
public:
	virtual ~Application() = default;

	virtual void Initialize() = 0;
	virtual void Run() = 0;

protected:
	virtual void ProcessInput() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	bool m_running = true;
	Window *m_window = nullptr;
};