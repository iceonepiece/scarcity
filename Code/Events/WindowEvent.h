#pragma once

#include "Event.h"

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(unsigned int width, unsigned int height)
		: m_width(width)
		, m_height(height)
	{}

	virtual EventType GetType() const override
	{
		return EventType::WindowResize;
	}

	virtual void Process(Application& app) override;

	unsigned int GetWidth() const { return m_width; }
	unsigned int GetHeight() const { return m_height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_width << ", " << m_height;
		return ss.str();
	}

private:
	unsigned int m_width;
	unsigned int m_height;
};
