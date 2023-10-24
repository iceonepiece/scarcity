#include "UIControl.h"
#include "Input/MouseCodes.h"

bool UIControl::HandleInput(float deltaTime, NewInput& input)
{
	if (Math::Contains(m_canvas->position, m_canvas->size, input.GetCursorPosition()))
	{
		if (!m_mouseOver)
		{
			std::cout << "MouseEnter\n";
			m_onMouseEnter.Invoke(this, *this);
			m_mouseOver = true;
		}

		if (!m_focused && input.GetMouseButtonDown(Mouse::ButtonLeft))
		{
			std::cout << "FocusReceived\n";
			m_onFocusReceived.Invoke(this, *this);
			m_focused = true;
			return true;
		}
	}
	else
	{
		if (m_mouseOver)
		{
			std::cout << "MouseLeave\n";
			m_onMouseLeave.Invoke(this, *this);
			m_mouseOver = false;
		}

		if (!m_focused && input.GetMouseButtonDown(Mouse::ButtonLeft))
		{
			m_onFocusLost.Invoke(this, *this);
			m_focused = false;
		}
	}

	return false;
}