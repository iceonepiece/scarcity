#pragma once

class Timer
{
public:
    static void Tick();
	static float GetDeltaTime();


private:
    static float m_deltaTime;
    static float m_lastFrame;
};
