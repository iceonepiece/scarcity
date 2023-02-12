#pragma once

#include <string>
#include <sstream>

enum class EventType
{
	None = 0,
	MouseButtonPressed, MouseMoved, MouseScrolled
};

class Event
{
public:
	virtual std::string ToString() const = 0;
	virtual EventType GetType() const = 0;

	bool m_resolved = false;
};