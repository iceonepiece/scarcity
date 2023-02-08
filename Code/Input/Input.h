#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "KeyCodes.h"
#include "MouseCodes.h"

class Game;

struct InputCommand
{
    int glfwKeyCode;
    KeyCode keyCode;
    int previousState;
    int currentState;
    bool isActive;

    InputCommand(int glfwKeyCode, KeyCode keyCode)
        : glfwKeyCode(glfwKeyCode)
        , keyCode(keyCode)
        , isActive(false)
        , previousState(GLFW_RELEASE)
        , currentState(GLFW_RELEASE)
    {
    }
};

struct MouseCommand
{
    int glfwMouseCode;
    MouseCode mouseCode;
    int previousState;
    int currentState;
    bool isActive;

    MouseCommand(int glfwMouseCode, MouseCode mouseCode)
        : glfwMouseCode(glfwMouseCode)
        , mouseCode(mouseCode)
        , isActive(false)
        , previousState(GLFW_RELEASE)
        , currentState(GLFW_RELEASE)
    {
    }
};


class Input
{
public:
    static void Init();
    static bool IsKeyPressed(KeyCode keyCode);
    static bool IsKeyHeld(KeyCode keyCode);
    static bool IsMouseButtonPressed(MouseCode mouseCode);
    static bool IsMouseButtonHeld(MouseCode mouseCode);
    static void PollInputs(GLFWwindow* glfwWindow);
    static void AddInputCommand(int glfwKeyCode, KeyCode keyCode);
    static void AddMouseCommand(int glfwMouseCode, MouseCode mouseCode);
    
    static glm::vec2 GetCursorPosition();
    static void SetCursorPosition(float xPos, float yPos);

private:
    static std::vector<InputCommand> s_inputCommands;
    static std::vector<MouseCommand> s_mouseCommands;
    static glm::vec2 s_cursorPosition;
};
