#pragma once

#include <glm/glm.hpp>

enum class SpriteShape
{
	None,
	Square,
	Circle
};

struct SpriteRendererComponent
{
	SpriteShape shape = SpriteShape::None;
	glm::vec4 color = glm::vec4 { 1.0f };
};