#pragma once

#include <vector>
#include "Core/Input.h"
#include "GLFW/glfw3.h"



struct GLFWInputCommand
{
    int glfwKeyCode;
    KeyCode keyCode;
    int previousState;
    int currentState;
    bool isActive;

    GLFWInputCommand(int glfwKeyCode, KeyCode keyCode)
        : glfwKeyCode(glfwKeyCode)
        , keyCode(keyCode)
        , isActive(false)
        , previousState(GLFW_RELEASE)
        , currentState(GLFW_RELEASE)
    {
    }
};

class GLFWInput : public Input
{
public:
	GLFWInput(GLFWwindow& window);

    virtual void Poll() override;
	virtual bool GetKey(KeyCode key) override;
	virtual bool GetKeyDown(KeyCode key) override;
	virtual bool GetKeyUp(KeyCode key) override;

protected:
	GLFWwindow& m_window;

    std::vector<GLFWInputCommand> m_inputCommands;
};