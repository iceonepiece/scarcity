#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Window.h"

class OpenGLWindow : public Window
{
public:
	OpenGLWindow(std::string title, int width, int height);
	virtual ~OpenGLWindow();
	
	virtual void ProcessInput() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual bool WindowShouldClose() override;

	GLFWwindow* GetGLFWwindow();

protected:
	GLFWwindow* m_glfwWindow;
};