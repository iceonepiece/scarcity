#pragma once

#include <glm/glm.hpp>
#include "UI/EventHandler.h"

struct UIComponent
{
	bool visible = true;
	bool enabled = true;
	bool focused = false;
	bool mouseOver = false;

	glm::vec4 color{ 1.0f };

	EventHandler<UIComponent&> mouseEnterHandler;
	EventHandler<UIComponent&> mouseLeaveHandler;
};