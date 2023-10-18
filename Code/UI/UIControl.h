#pragma once

#include <glm/glm.hpp>
#include "Input/NewInput.h"
#include "UI/EventHandler.h"
#include "Math/Math.h"
#include "Graphics/Renderer.h"

class UIControl
{
public:
	void SetPosition(const glm::vec2& position)
	{
		m_position = position;
	}

	void SetSize(const glm::vec2& size)
	{
		m_size = size;
	}

	void SetBackgroundColor(const glm::vec4& color)
	{
		m_backgroundColor = color;
	}

	void Update(float deltaTime)
	{
		m_backgroundColor.a = m_alpha;
		m_foregroundColor.a = m_alpha;
	}

	virtual void Draw(Renderer& renderer)
	{

	}

	virtual bool HandleInput(float deltaTime, NewInput& input);

protected:
	glm::vec2 m_position;
	glm::vec2 m_size;

	float m_alpha;

	glm::vec4 m_foregroundColor;
	glm::vec4 m_backgroundColor;

	bool m_visible;
	bool m_enabled;
	bool m_focused = false;
	bool m_mouseOver = false;

	glm::vec2 m_viewportSize;

	EventHandler<const UIControl&> m_onFocusReceived;
	EventHandler<const UIControl&> m_onFocusLost;
	EventHandler<const UIControl&> m_onMouseEnter;
	EventHandler<const UIControl&> m_onMouseLeave;
};