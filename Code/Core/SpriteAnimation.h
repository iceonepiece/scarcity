#pragma once

#include <vector>
#include "Sprite.h"

class SpriteAnimation
{
public:
	Texture& texture;
	std::vector<glm::vec2> positions;
	glm::vec2 size;

	int currentFrame;
	float frameSpeed;
	float timer;
	float scale;

	SpriteAnimation(Texture& texture, std::vector<glm::vec2> positions, glm::vec2 size, float frameSpeed, float scale = 1)
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