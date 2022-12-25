#include "Input.h"
#include "../Core/Game.h"

std::vector<InputCommand> Input::s_inputCommands;

void Input::Init()
{
    AddInputCommand(GLFW_KEY_ESCAPE, Key::Escape);
    AddInputCommand(GLFW_KEY_UP, Key::Up);
    AddInputCommand(GLFW_KEY_DOWN, Key::Down);
    AddInputCommand(GLFW_KEY_LEFT, Key::Left);
    AddInputCommand(GLFW_KEY_RIGHT, Key::Right);
    AddInputCommand(GLFW_KEY_SPACE, Key::Space);
    AddInputCommand(GLFW_KEY_ENTER, Key::Enter);
    AddInputCommand(GLFW_KEY_F, Key::F);
    AddInputCommand(GLFW_KEY_Z, Key::Z);
    AddInputCommand(GLFW_KEY_X, Key::X);
    AddInputCommand(GLFW_KEY_Q, Key::Q);
}

void Input::AddInputCommand(int glfwKeyCode, KeyCode keyCode)
{
    s_inputCommands.push_back(InputCommand(glfwKeyCode, keyCode));
}

void Input::PollInputs(GLFWwindow* glfwWindow)
{
    for (auto& inputCommand : s_inputCommands)
    {
        inputCommand.previousState = inputCommand.currentState;
        inputCommand.currentState = glfwGetKey(glfwWindow, inputCommand.glfwKeyCode);
    }
}

bool Input::IsKeyPressed(KeyCode keyCode)
{
    for (auto& inputCommand : s_inputCommands)
    {
        if (inputCommand.keyCode == keyCode
            && inputCommand.previousState == GLFW_RELEASE
            && inputCommand.currentState == GLFW_PRESS
        )
            return true;
    }

    return false;
}

bool Input::IsKeyHeld(KeyCode keyCode)
{
    for (auto& inputCommand : s_inputCommands)
    {
        if (inputCommand.keyCode == keyCode
            && inputCommand.previousState == GLFW_PRESS
            && inputCommand.currentState == GLFW_PRESS
        )
            return true;
    }

    return false;
}
