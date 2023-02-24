#pragma once

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "UIComponent.h"

#include "../Graphics/FontSystem.h"

class UIText : public UIComponent
{
public:
	UIText(glm::vec2 position, std::string text, float scale, glm::vec3 color, UIAlignment alignment = UIAlignment::NONE)
		: UIComponent(position)
		, text(text)
		, scale(scale)
		, color(color)
	{
		this->alignment = alignment;
	}

	virtual void Draw() override
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		FontSystem::RenderText(this);

		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	}

	virtual ~UIText() {}

	std::string text;
	float scale;
	glm::vec3 color;
};