#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "KeyCodes.h"

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

class Input
{
public:
    static void Init();
    static bool IsKeyPressed(KeyCode keyCode);
    static bool IsKeyHeld(KeyCode keyCode);
    static void PollInputs(GLFWwindow* glfwWindow);
    static void AddInputCommand(int glfwKeyCode, KeyCode keyCode);

private:
    static std::vector<InputCommand> s_inputCommands;
};
