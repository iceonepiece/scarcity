#pragma once

#include "Core/Application.h"
#include "Core/ResourceAPI.h"
#include "Platforms/OpenGL/OpenGLWindow.h"
#include "Platforms/OpenGL/OpenGLRenderer.h"
#include "Platforms/OpenGL/OpenGLResourceManager.h"
#include "Platforms/GLFW/GLFWInput.h"
#include "Utils/FileDialog.h"
#include "ImGui/ImGuiManager.h"
#include "GameLayer.h"

class SandboxApplication : public Application
{
public:
    SandboxApplication()
        : Application({"Sandbox"})
    {}

    virtual ~SandboxApplication() override
    {
        std::cout << "Sandbox Application destroyed" << std::endl;
    }

	virtual void Initialize(std::string title, int width, int height) override
	{   
        m_input = std::make_unique<GLFWInput>(*((GLFWwindow*)m_window->GetNativeWindow()));
        Input::Init();

        ResourceAPI::Initialize(new OpenGLResourceManager());

        m_imguiManager = std::make_unique<ImGuiManager>(*this);

        FileDialog::SetNativeWindow(m_window->GetNativeWindow());

        AddLayer(std::make_unique<GameLayer>());
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