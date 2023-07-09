#include "KeyEvent.h"
#include "Core/Application.h"

KeyPressedEvent::KeyPressedEvent(KeyCode keycode)
	: KeyEvent(keycode)
{}

std::string KeyPressedEvent::ToString() const
{
	return "Key Pressed Event";
}

void KeyPressedEvent::Process(Application& app)
{
	app.OnKeyPressed(*this);
}

EventType KeyPressedEvent::GetType() const
{
	return EventType::KeyPressed;
}