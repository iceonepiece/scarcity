#pragma once

#include <glm/glm.hpp>

struct TransformComponent
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	TransformComponent()
		: position(0)
		, rotation(0)
		, scale(1)
	{}
};