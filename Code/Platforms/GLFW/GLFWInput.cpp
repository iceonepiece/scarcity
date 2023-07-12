#include "GLFWInput.h"
#include <map>

std::map<KeyCode, int> glfwKeyMap {
    { Key::Escape, GLFW_KEY_ESCAPE },
    { Key::Up, GLFW_KEY_UP },
    { Key::Down, GLFW_KEY_DOWN },
    { Key::Left, GLFW_KEY_LEFT },
    { Key::Right, GLFW_KEY_RIGHT },
    { Key::Space, GLFW_KEY_SPACE },
    { Key::Enter, GLFW_KEY_ENTER },
    { Key::W, GLFW_KEY_W },
    { Key::A, GLFW_KEY_A },
    { Key::S, GLFW_KEY_S },
    { Key::D, GLFW_KEY_D },
    { Key::F, GLFW_KEY_F },
    { Key::Z, GLFW_KEY_Z },
    { Key::X, GLFW_KEY_X },
    { Key::Q, GLFW_KEY_Q }
};

GLFWInput::GLFWInput(GLFWwindow& window)
	: m_window(window)
{
    for (auto& key : glfwKeyMap)
    {
        m_inputCommands.push_back(GLFWInputCommand(key.second, key.first));
    }
}

void GLFWInput::Poll()
{
    for (auto& inputCommand : m_inputCommands)
    {
        inputCommand.previousState = inputCommand.currentState;
        inputCommand.currentState = glfwGetKey(&m_window, inputCommand.glfwKeyCode);
    }
}

bool GLFWInput::GetKey(KeyCode key)
{
    for (auto& command : m_inputCommands)
    {
        if (command.keyCode == key)
            return command.currentState == GLFW_PRESS;
    }

    return false;
}

bool GLFWInput::GetKeyDown(KeyCode key)
{
    for (auto& command : m_inputCommands)
    {
        if (command.keyCode == key)
            return command.previousState == GLFW_RELEASE && command.currentState == GLFW_PRESS;
    }

    return false;
}

bool GLFWInput::GetKeyUp(KeyCode key)
{
    for (auto& command : m_inputCommands)
    {
        if (command.keyCode == key)
            return command.currentState == GLFW_RELEASE;
    }

    return false;
}