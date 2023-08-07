#include "Input.h"

std::vector<InputCommand> Input::s_inputCommands;
std::vector<MouseCommand> Input::s_mouseCommands;
glm::vec2 Input::s_cursorPosition;

void Input::Init()
{
    AddInputCommand(GLFW_KEY_ESCAPE, Key::Escape);
    AddInputCommand(GLFW_KEY_UP, Key::Up);
    AddInputCommand(GLFW_KEY_DOWN, Key::Down);
    AddInputCommand(GLFW_KEY_LEFT, Key::Left);
    AddInputCommand(GLFW_KEY_RIGHT, Key::Right);
    AddInputCommand(GLFW_KEY_SPACE, Key::Space);
    AddInputCommand(GLFW_KEY_ENTER, Key::Enter);
    AddInputCommand(GLFW_KEY_W, Key::W);
    AddInputCommand(GLFW_KEY_A, Key::A);
    AddInputCommand(GLFW_KEY_S, Key::S);
    AddInputCommand(GLFW_KEY_D, Key::D);
    AddInputCommand(GLFW_KEY_F, Key::F);
    AddInputCommand(GLFW_KEY_Z, Key::Z);
    AddInputCommand(GLFW_KEY_X, Key::X);
    AddInputCommand(GLFW_KEY_Q, Key::Q);

    AddMouseCommand(GLFW_MOUSE_BUTTON_LEFT, Mouse::ButtonLeft);
    AddMouseCommand(GLFW_MOUSE_BUTTON_RIGHT, Mouse::ButtonRight);
    AddMouseCommand(GLFW_MOUSE_BUTTON_MIDDLE, Mouse::ButtonMiddle);
}

void Input::AddInputCommand(int glfwKeyCode, KeyCode keyCode)
{
    s_inputCommands.push_back(InputCommand(glfwKeyCode, keyCode));
}

void Input::AddMouseCommand(int glfwMouseCode, MouseCode mouseCode)
{
    s_mouseCommands.push_back(MouseCommand(glfwMouseCode, mouseCode));
}

void Input::PollInputs(GLFWwindow* glfwWindow)
{
    for (auto& inputCommand : s_inputCommands)
    {
        inputCommand.previousState = inputCommand.currentState;
        inputCommand.currentState = glfwGetKey(glfwWindow, inputCommand.glfwKeyCode);
    }

    for (auto& mouseCommand : s_mouseCommands)
    {
        mouseCommand.previousState = mouseCommand.currentState;
        mouseCommand.currentState = glfwGetMouseButton(glfwWindow, mouseCommand.glfwMouseCode);
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

bool Input::IsMouseButtonPressed(MouseCode mouseCode)
{
    for (auto& mouseCommand : s_mouseCommands)
    {
        if (mouseCommand.mouseCode == mouseCode
            && mouseCommand.previousState == GLFW_RELEASE
            && mouseCommand.currentState == GLFW_PRESS
            )
            return true;
    }

    return false;
}

bool Input::IsMouseButtonHeld(MouseCode mouseCode)
{
    for (auto& mouseCommand : s_mouseCommands)
    {
        if (mouseCommand.mouseCode == mouseCode
            && mouseCommand.previousState == GLFW_PRESS
            && mouseCommand.currentState == GLFW_PRESS
            )
            return true;
    }

    return false;
}

void Input::SetCursorPosition(float xPos, float yPos)
{
    s_cursorPosition.x = xPos;
    s_cursorPosition.y = yPos;
}

glm::vec2 Input::GetCursorPosition()
{
    return s_cursorPosition;
}
