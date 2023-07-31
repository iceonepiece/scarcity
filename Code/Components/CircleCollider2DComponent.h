#pragma once

#include <box2d/b2_body.h>

struct CircleCollider2DComponent
{
	glm::vec2 offset = { 0.0f, 0.0f };
	float radius = 0.5;
};