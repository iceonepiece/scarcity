#pragma once

#include <vector>
#include "Input/NewInput.h"
#include "GLFW/glfw3.h"

class GLFWInput : public NewInput
{
public:
	GLFWInput(GLFWwindow& window);

    virtual void Poll() override;

protected:
	GLFWwindow& m_window;
};