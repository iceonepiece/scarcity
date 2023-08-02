#pragma once

#include <string>
#include <sstream>

class Application;

enum class EventType
{
	None = 0,
	MouseButtonPressed, MouseButtonReleased,
	MouseMoved, MouseScrolled,
	KeyPressed, KeyReleased,
	WindowResize, WindowClose
};

class Event
{
public:
	virtual std::string ToString() const = 0;
	virtual EventType GetType() const = 0;
	virtual void Process(Application& app) {}

	bool handled = false;
};
