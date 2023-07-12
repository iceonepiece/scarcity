#include <iostream>
#include "OpenGLWindow.h"
#include "Graphics/Renderer.h"
#include "Graphics/RendererAPI.h"
#include "Input/Input.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

OpenGLWindow::OpenGLWindow(Application* app, std::string title, int width, int height)
	: Window(app, title, width, height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
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
  
    glfwSetWindowUserPointer(m_glfwWindow, m_app);

    glfwSetWindowSizeCallback(m_glfwWindow, [](GLFWwindow* window, int width, int height)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);
        app.OnWindowResize(width, height);
    });

    glfwSetWindowCloseCallback(m_glfwWindow, [](GLFWwindow* window)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);
        app.OnWindowClose();
    });


    glfwSetKeyCallback(m_glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);

        switch (action)
        {
            case GLFW_PRESS:
            {
                KeyPressedEvent event(key);
                app.OnKeyPressed(event);
            }
        }
    });

    glfwSetMouseButtonCallback(m_glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);
        switch (action)
        {
            case GLFW_PRESS:    app.OnMouseButtonPressed(button);   break;
            case GLFW_RELEASE:  app.OnMouseButtonReleased(button);  break;
        }
    });

    glfwSetCursorPosCallback(m_glfwWindow, [](GLFWwindow* window, double xPos, double yPos)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);
        app.OnMouseMoved((float)xPos, (float)yPos);
    });
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

    //Renderer::SetScreenSize(width, height, xOffset, yOffset);
    //RendererAPI::SetScreenSize(width, height, xOffset, yOffset);

    glViewport(xOffset, yOffset, width, height);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::Render()
{
    glfwSwapBuffers(m_glfwWindow);
}