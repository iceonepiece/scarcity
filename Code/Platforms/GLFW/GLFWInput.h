#pragma once

#include <vector>
#include "Input/Input.h"
#include "GLFW/glfw3.h"

class GLFWInput : public Input
{
public:
	GLFWInput(GLFWwindow& window);

    virtual void Poll() override;

protected:
	GLFWwindow& m_window;
};