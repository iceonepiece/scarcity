#pragma once

class Window
{
public:
	Window(int width = 1280, int height = 720)
		: m_width(width)
		, m_height(height)
		, m_ratio(width / (float)height)
	{}

	virtual ~Window() = default;
	virtual void ProcessInput() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual bool WindowShouldClose() = 0;

protected:
	int m_width;
	int m_height;
	float m_ratio;
};