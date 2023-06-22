#include <GLFW/glfw3.h>
#include <iostream>
#include "Timer.h"

double Timer::m_deltaTime = 0.0;
double Timer::m_lastFrame = 0.0;
double Timer::m_lastSecond = 0.0;
int Timer::m_frameCount = 0;

void Timer::Tick()
{
	double currentFrame = glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;
}

double Timer::GetDeltaTime()
{
	return m_deltaTime;
}

void Timer::DisplayFPS()
{
	m_frameCount++;

	if (m_lastFrame - m_lastSecond >= 1.0)
	{
		std::cout << "FPS: " << m_frameCount << std::endl;
		m_frameCount = 0;
		m_lastSecond = m_lastFrame;
	}
}
