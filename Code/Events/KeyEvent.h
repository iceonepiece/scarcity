#pragma once

#include "Event.h"
#include "Input/KeyCodes.h"

class KeyEvent : public Event
{
public:
	KeyCode GetKeyCode() const { return m_keyCode; }

protected:
	KeyEvent(const KeyCode keycode)
		: m_keyCode(keycode)
	{}

	KeyCode m_keyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(KeyCode keycode);
	virtual std::string ToString() const override;
	virtual void Process(Application& app) override;
	virtual EventType GetType() const override;
};
