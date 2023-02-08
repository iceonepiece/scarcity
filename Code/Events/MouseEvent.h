#pragma once

#include "Event.h"

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(float x, float y)
		: m_x(x)
		, m_y(y)

	{
	}

	EventType GetType() const override
	{
		return EventType::MouseMoved;
	}

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_x << ", " << m_y;
		return ss.str();
	}

private:
	float m_x;
	float m_y;
};