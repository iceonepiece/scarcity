#pragma once

#include <glm/glm.hpp>

#include "Scene/Scene.h"

class UIComponent
{
public:
	UIComponent(glm::vec2 position)
		: position(position)
		, autoScale(false)
		, alignment(UIAlignment::NONE)
		, scene(nullptr)
		, parent(nullptr)
		, m_active(true)
	{
	}

	virtual void Update(float deltaTime) {}
	virtual void Draw() = 0;

	virtual ~UIComponent()
	{
	}

	void SetActive(bool active)
	{
		m_active = active;
	}

	bool IsActive()
	{
		return m_active;
	}

	void SetAligment(UIAlignment alignment)
	{
		this->alignment = alignment;
	}

	glm::vec2 position;
	Scene* scene;
	UIComponent* parent;
	UIAlignment alignment;
	bool autoScale;

private:
	bool m_active;
};