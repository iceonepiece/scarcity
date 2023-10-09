#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Animations/Sprite.h"

/*
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
*/

struct SpriteAnimation
{
	Texture& texture = g_nullTexture;
	int startFrame;
	int maxFame;
	int rows;
	int cols;
	float frameSpeed;
	glm::vec2 scale { 1.0f, 1.0f };
	int currentFrame = 0;
	float timer = 0.0f;

	Sprite GetSprite()
	{
		int x = currentFrame % cols;
		int y = (rows - 1) - currentFrame / cols;
		float u = texture.GetWidth() / (float)cols;
		float v = texture.GetHeight() / (float)rows;

		return Sprite{};
		//return Sprite { &(texture), {u, v}, {x, y} };
	}
};