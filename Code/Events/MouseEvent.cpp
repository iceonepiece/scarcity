#include "MouseEvent.h"
#include "Core/Application.h"

MouseMovedEvent::MouseMovedEvent(float x, float y)
	: m_x(x)
	, m_y(y)

{
}

EventType MouseMovedEvent::GetType() const
{
	return EventType::MouseMoved;
}

std::string MouseMovedEvent::ToString() const
{
	std::stringstream ss;
	ss << "MouseMovedEvent: " << m_x << ", " << m_y;
	return ss.str();
}

void MouseMovedEvent::Process(Application& app)
{
	app.OnMouseMoved(*this);
}


MouseScrolledEvent::MouseScrolledEvent(float x, float y)
	: m_x(x)
	, m_y(y)

{
}

EventType MouseScrolledEvent::GetType() const
{
	return EventType::MouseScrolled;
}

std::string MouseScrolledEvent::ToString() const
{
	std::stringstream ss;
	ss << "MouseScrolledEvent: " << m_x << ", " << m_y;
	return ss.str();
}

void MouseScrolledEvent::Process(Application& app)
{

}

void MouseButtonPressedEvent::Process(Application& app)
{
	app.OnMouseButtonPressed(*this);
}


