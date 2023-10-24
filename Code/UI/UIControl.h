#pragma once

#include <glm/glm.hpp>
#include "Input/NewInput.h"
#include "UI/EventHandler.h"
#include "Math/Math.h"
#include "Graphics/Renderer.h"
#include "Components/UIComponents.h"
#include "Scene/ScriptableEntity.h"

class UIControl
{
public:
	UIControl()
		: m_canvas(&defaultCanvas)
	{}

	UIControl(CanvasComponent* canvas)
		: m_canvas(canvas)
	{}

	void SetCanvas(CanvasComponent* canvas)
	{
		m_canvas = canvas;
	}

	void SetPosition(const glm::vec2& position)
	{
		m_canvas->position = position;
	}

	void SetSize(const glm::vec2& size)
	{
		m_canvas->size = size;
	}

	void SetBackgroundColor(const glm::vec4& color)
	{
		m_backgroundColor = color;
	}

	void SetScriptableEntity(ScriptableEntity* scriptable)
	{
		m_scriptable = scriptable;

		if (m_scriptable != nullptr)
			m_scriptable->ExportFunctions();
	}

	void Update(float deltaTime)
	{
		//m_backgroundColor.a = m_alpha;
		//m_foregroundColor.a = m_alpha;
	}

	virtual void Draw(Renderer& renderer)
	{

	}

	virtual bool HandleInput(float deltaTime, NewInput& input);


public:
	inline static CanvasComponent defaultCanvas;
	ScriptableEntity* m_scriptable = nullptr;

protected:
	//glm::vec2 m_position;
	//glm::vec2 m_size;
	CanvasComponent* m_canvas = nullptr;

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
	EventHandler<UIControl&> m_onMouseEnter;
	EventHandler<UIControl&> m_onMouseLeave;
};