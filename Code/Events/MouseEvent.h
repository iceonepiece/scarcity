#pragma once

#include "Core/Application.h"
#include "Event.h"

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
