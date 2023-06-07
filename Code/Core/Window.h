#pragma once

#include <string>

class Window
{
public:
	Window(std::string title, int width = 1280, int height = 720)
		: m_title(title)
		, m_width(width)
		, m_height(height)
		, m_ratio(width / (float)height)
	{}

	virtual ~Window() = default;
	virtual void ProcessInput() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual bool WindowShouldClose() = 0;

protected:
	std::string m_title;
	int m_width;
	int m_height;
	float m_ratio;
};