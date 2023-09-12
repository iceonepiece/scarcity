#include <chrono>
#include <iostream>
#include "Timer.h"

double Timer::m_deltaTime = 0.0;
double Timer::m_lastFrame = 0.0;
double Timer::m_lastSecond = 0.0;
int Timer::m_frameCount = 0;

void Timer::Tick()
{
	auto currentTime = std::chrono::high_resolution_clock::now();

	auto currentTimePoint = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = currentTimePoint.time_since_epoch();
	double currentTimeInSeconds = duration.count();

	m_deltaTime = currentTimeInSeconds - m_lastFrame;
	m_lastFrame = currentTimeInSeconds;
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
