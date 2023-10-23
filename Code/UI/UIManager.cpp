#include "UIManager.h"
#include "Math/Math.h"
#include <iostream>

void UIManager::ProcessButton(ButtonComponent& button, NewInput& input)
{

}

bool UIManager::HandleInput(CanvasComponent& canvas, ButtonComponent& button, NewInput& input)
{
	if (Math::Contains(canvas.position, canvas.size, input.GetCursorPosition()))
	{
		if (input.GetMouseButtonDown(Mouse::ButtonLeft))
		{
			button.mousePressedHandler.Invoke(&button, button);
		}

		if (!button.mouseOver)
		{
			std::cout << "Mouse Enter\n";
			button.mouseEnterHandler.Invoke(&button, button);
			button.mouseOver = true;
		}
		if (!button.focused && input.GetMouseButtonDown(Mouse::ButtonLeft))
		{
			std::cout << "Focus Received\n";
			//m_onFocusReceived.Invoke(this, *this);
			button.focused = true;
			return true;
		}
	}
	else
	{
		if (button.mouseOver)
		{
			std::cout << "Mouse Leave\n";
			button.mouseLeaveHandler.Invoke(&button, button);
			button.mouseOver = false;
		}

		if (!button.focused && input.GetMouseButtonDown(Mouse::ButtonLeft))
		{
			//m_onFocusLost.Invoke(this, *this);
			button.focused = false;
		}
	}

	return false;
}