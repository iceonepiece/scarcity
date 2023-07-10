#pragma once

#include <glm/glm.hpp>

struct TransformComponent
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	TransformComponent(const glm::vec3& p, const glm::vec3& r, const glm::vec3& s)
		: position(p)
		, rotation(r)
		, scale(s)
	{
	}

	TransformComponent()
		: position(0)
		, rotation(0)
		, scale(1)
	{}
};