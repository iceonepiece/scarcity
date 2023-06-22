#pragma once

#include <vector>
#include "../Graphics/Texture.h"

struct Animation2D
{
	Texture& texture;
	int startFrame;
	int maxFame;
	int rows;
	int cols;
	float frameSpeed;
};