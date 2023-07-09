#pragma once

#include "Core/Application.h"
#include "Event.h"
#include "Input/MouseCodes.h"

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(float x, float y);

	EventType GetType() const override;

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

	std::string ToString() const override;
	virtual void Process(Application& app) override;

private:
	float m_x;
	float m_y;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(float x, float y);

	EventType GetType() const override;

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

	std::string ToString() const override;
	virtual void Process(Application& app) override;

private:
	float m_x;
	float m_y;
};

class MouseButtonEvent : public Event
{
public:
	MouseCode GetMouseButton() const { return m_button; }

	virtual void Process(Application& app) override {}

protected:
	MouseButtonEvent(const MouseCode button)
		: m_button(button) {}

	MouseCode m_button;
};


class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(const MouseCode button)
		: MouseButtonEvent(button) {}

	virtual EventType GetType() const override
	{
		return EventType::MouseButtonPressed;
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_button;
		return ss.str();
	}
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(const MouseCode button)
		: MouseButtonEvent(button) {}

	virtual EventType GetType() const override
	{
		return EventType::MouseButtonReleased;
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_button;
		return ss.str();
	}
};
