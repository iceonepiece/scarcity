#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Window.h"
#include "Core/Application.h"

class OpenGLWindow : public Window
{
public:
	OpenGLWindow(const WindowConfigs& configs);
	OpenGLWindow(Application* app, std::string title, int width, int height);
	virtual ~OpenGLWindow();
	
	virtual void ProcessInput() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual bool WindowShouldClose() override;

	virtual void SetEventCallback(EventCallbackFn callback) override
	{
		
		m_data.EventCallback = callback;
		std::cout << "Set Event Callback" << std::endl;
		std::cout << "XXXX: " << &(m_data.EventCallback) << std::endl;
	}

	virtual void SetTitle(const std::string& title) override;
	virtual void SetCursorShape(CursorShape shape) override;

	virtual void SetVSync(bool enabled) override;
	virtual bool IsVSync() const override;

	virtual void* GetNativeWindow() const override { return m_glfwWindow; }

	GLFWwindow* GetGLFWwindow();

protected:
	GLFWwindow* m_glfwWindow;
	GLFWcursor* m_cursor;

	bool m_vsync;
};