#pragma once

#include "UIComponent.h"
#include "../Graphics/Renderer.h"

class UIContainer : public UIComponent
{
public:
	UIContainer(glm::vec2 position, glm::vec2 scale, glm::vec4 color)
		: UIComponent(position)
		, m_scale(scale)
		, m_color(color)
	{

	}
	
	virtual void Draw() override
	{
		//Renderer::DrawQuadUI(position, m_scale, m_color, alignment);
	}

protected:
	std::vector<UIComponent*> m_children;
	glm::vec2 m_scale;
	glm::vec4 m_color;
};