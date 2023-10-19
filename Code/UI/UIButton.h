#pragma once

#include "UI/UIControl.h"

class UIButton : public UIControl
{
public:
	enum State
	{
		Default,
		Pressed,
		Disabled,
		StateCount
	};

	UIButton();
	/*
	UIButton(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
		: m_state(Default)
	{
		//m_canvas.position = position;
		//m_canvas.size = size;
		//m_backgroundColor = color;

		m_onMouseEnter += [&](void*, const UIControl& control)
		{
				std::cout << "Button OnMouseEnter()\n";
			m_backgroundColor = color * 1.5f;
		};

		m_onMouseLeave += [&](void*, const UIControl& control)
		{
				std::cout << "Button OnMouseEnter()\n";
			m_backgroundColor = color;
		};
	}
	*/

	EventHandler<const UIButton&>& OnPressed()
	{
		return m_onPressed;
	}

	EventHandler<const UIButton&>& OnReleased()
	{
		return m_onReleased;
	}

	virtual void Draw(Renderer& renderer) override
	{
		renderer.DrawQuadUI(m_canvas->position, m_canvas->size, m_backgroundColor, UIAlignment::NONE);
	}

	virtual bool HandleInput(float deltaTime, NewInput& input) override
	{
		return UIControl::HandleInput(deltaTime, input);
	}


protected:
	State m_state;

	EventHandler<const UIButton&> m_onPressed;
	EventHandler<const UIButton&> m_onReleased;
};