#include <iostream>
#include "OpenGLWindow.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/RendererAPI.h"
#include "../Input/Input.h"

OpenGLWindow::OpenGLWindow(int width, int height)
	: Window(width, height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_glfwWindow = glfwCreateWindow(m_width, m_height, "GLFW Window", NULL, NULL);
    if (m_glfwWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_glfwWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

OpenGLWindow::~OpenGLWindow()
{
    glfwDestroyWindow(m_glfwWindow);
    glfwTerminate();
}

GLFWwindow* OpenGLWindow::GetGLFWwindow()
{
    return m_glfwWindow;
}

bool OpenGLWindow::WindowShouldClose()
{
    return glfwWindowShouldClose(m_glfwWindow);
}

void OpenGLWindow::ProcessInput()
{
    glfwPollEvents();
    Input::PollInputs(m_glfwWindow);        
}

void OpenGLWindow::PreRender()
{
    glfwGetFramebufferSize(m_glfwWindow, &m_width, &m_height);

    float windowRatio = m_width / (float)m_height;

    int width = windowRatio > m_ratio ? m_height * m_ratio : m_width;
    int height = windowRatio < m_ratio ? m_width / m_ratio : m_height;

    int xOffset = (m_width - width) / 2;
    int yOffset = (m_height - height) / 2;

    Renderer::SetScreenSize(width, height, xOffset, yOffset);
    RendererAPI::SetScreenSize(width, height, xOffset, yOffset);

    glViewport(xOffset, yOffset, width, height);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::Render()
{
    glfwSwapBuffers(m_glfwWindow);
}