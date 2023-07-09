#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Window.h"
#include "Core/Application.h"

class OpenGLWindow : public Window
{
public:
	OpenGLWindow(std::string title, int width, int height);
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

	GLFWwindow* GetGLFWwindow();

protected:
	GLFWwindow* m_glfwWindow;

	struct WindowData
	{
		std::string title;
		unsigned int width, height;

		//Application* app;

		EventCallbackFn EventCallback;
	};

	WindowData m_data;
};