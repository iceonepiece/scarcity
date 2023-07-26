#pragma once

#include <glm/glm.hpp>

struct Shape2D
{
	glm::vec4 color = glm::vec4 { 1.0f };
	glm::vec2 position = glm::vec2{ 0.0f };
	glm::vec2 scale = glm::vec2{ 1.0f };
	float angle = 0.0f;
};

struct Quad2D : public Shape2D
{
};

struct Circle2D : public Shape2D
{
};