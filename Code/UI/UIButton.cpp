#include "UIButton.h"

UIButton::UIButton()
{
	m_onMouseEnter += [&](void*, UIControl& control)
	{
		std::cout << "Button OnMouseEnter()\n";
	};

	m_onMouseLeave += [&](void*, UIControl& control)
	{
		std::cout << "Button OnMouseLeave()\n";
	};
}