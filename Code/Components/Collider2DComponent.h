#pragma once

enum class ColliderType
{
	Box,
	Circle,
};

struct Collider2DComponent
{
	uint16_t layer = 0;
	bool isTrigger = false;
	glm::vec2 offset = { 0.0f, 0.0f };
	float density = 1.0f;
	float friction = 0.0f;

	virtual ColliderType GetType() const = 0;
};
