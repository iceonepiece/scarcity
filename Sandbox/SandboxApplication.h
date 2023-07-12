#pragma once

#include "Core/Application.h"
#include "Platforms/OpenGL/OpenGLWindow.h"
#include "Platforms/OpenGL/OpenGLRenderer.h"

class SandboxApplication : public Application
{
public:
    virtual ~SandboxApplication() override
    {
        std::cout << "Sandbox Application destroyed" << std::endl;
    }

	virtual void Initialize(std::string title, int width, int height) override
	{
        m_window = std::make_unique<OpenGLWindow>(this, title, width, height);

        m_renderer = std::make_unique<OpenGLRenderer>();
        m_renderer->Initialize();
	}

	virtual void Run() override
	{
        while (m_running)
        {
            ProcessInput();
            Update();
            Render();
        }
	}

protected:
	virtual void ProcessInput() override
    {
        m_window->ProcessInput();

        if (m_window->WindowShouldClose())
            m_running = false;
    }

	//virtual void Update() override {}
	//virtual void Render() override {}
};