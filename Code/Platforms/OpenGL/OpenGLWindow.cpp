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
    
    glfwSetScrollCallback(m_glfwWindow, [](GLFWwindow* window, double xPos, double yPos)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);
        app.OnMouseScroll((float)xPos, (float)yPos);
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
    int rawWidth;
    int rawHeight;

    glfwGetFramebufferSize(m_glfwWindow, &rawWidth, &rawHeight);

    float windowRatio = rawWidth / (float)rawHeight;

    int width = windowRatio > m_ratio ? rawHeight * m_ratio : rawWidth;
    int height = windowRatio < m_ratio ? rawWidth / m_ratio : rawHeight;
    int xOffset = (rawWidth - width) / 2;
    int yOffset = (rawHeight - height) / 2;

    m_data.width = width;
    m_data.height = height;
    m_data.xOffset = xOffset;
    m_data.yOffset = yOffset;

    //glViewport(xOffset, yOffset, width, height);
    glViewport(0, 0, rawWidth, rawHeight);

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::Render()
{
    glfwSwapBuffers(m_glfwWindow);
}