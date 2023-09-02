#pragma once

#include "File/FileSystem.h"

enum class SpriteMode
{
	Single,
	Multiple
};

struct SpriteResource : public Resource
{
	virtual std::string GetTypeString() { return "Image"; }

	SpriteMode mode = SpriteMode::Single;
	unsigned int cols = 0;
	unsigned int rows = 0;
};