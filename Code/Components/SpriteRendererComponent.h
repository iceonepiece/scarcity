#pragma once

#include <glm/glm.hpp>

enum SpriteShape
{
	Shape_None,
	Shape_Square,
	Shape_Circle
};

struct SpriteRendererComponent
{
	SpriteShape shape = Shape_None;
	glm::vec4 color = glm::vec4 { 1.0f };
};