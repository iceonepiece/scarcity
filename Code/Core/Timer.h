#pragma once

class Timer
{
public:
    static void Tick();
	static double GetDeltaTime();
    static void DisplayFPS();

private:
    static double m_deltaTime;
    static double m_lastFrame;

    static double m_lastSecond;
    static int m_frameCount;
};
