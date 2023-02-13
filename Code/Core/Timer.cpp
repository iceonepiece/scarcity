#include <GLFW/glfw3.h>
#include "Timer.h"

float Timer::m_deltaTime = 0.0f;
float Timer::m_lastFrame = 0.0f;

void Timer::Tick()
{
	float currentFrame = glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;
}

float Timer::GetDeltaTime()
{
	return m_deltaTime;
}