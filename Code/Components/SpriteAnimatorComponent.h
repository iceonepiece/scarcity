#pragma once

#include <vector>
#include "../Core/Sprite.h"
#include "../Core/SpriteAnimation.h"

struct SpriteAnimatorComponent
{
	std::unordered_map<std::string, SpriteAnimation*> animations;
	std::string currentAnimationName;

	SpriteAnimatorComponent(std::string currentAnimationName, std::unordered_map<std::string, SpriteAnimation*> animations)
		: currentAnimationName(currentAnimationName)
		, animations(animations)
	{}

	~SpriteAnimatorComponent()
	{
		for (auto anime : animations)
		{
			delete anime.second;
		}
		animations.clear();
	}
};
