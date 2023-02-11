#pragma once

#include <vector>
#include "../Core/Sprite.h"

struct SpriteAnimationComponent
{
	Texture& texture;
	std::vector<glm::vec2> positions;
	glm::vec2 size;

	int currentFrame;
	float frameSpeed;
	float timer;
	float scale;

	SpriteAnimationComponent(Texture& texture, std::vector<glm::vec2> positions, glm::vec2 size, float frameSpeed, float scale = 1)
		: texture(texture)
		, positions(positions)
		, size(size)
		, frameSpeed(frameSpeed)
		, timer(0)
		, currentFrame(0)
		, scale(scale)
	{

	}
};
