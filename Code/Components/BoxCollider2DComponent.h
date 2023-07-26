#pragma once

#include <glm/glm.hpp>

struct BoxCollider2DComponent
{
	glm::vec2 offset = { 0.0f, 0.0f };
	glm::vec2 size = { 0.5f, 0.5f };
};