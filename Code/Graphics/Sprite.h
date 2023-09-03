#pragma once

#include "File/FileSystem.h"
#include "Graphics/Texture.h"

enum class SpriteMode
{
	Single,
	Multiple
};

struct SpriteResource : public Resource
{
	virtual std::string GetTypeString() { return "Image"; }

	Texture* texture;
	SpriteMode mode = SpriteMode::Single;
	int cols = 0;
	int rows = 0;
};