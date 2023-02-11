#pragma once

#include <vector>
#include "Sprite.h"

class SpriteAnimation
{
public:
	SpriteAnimation()
	{
	}



private:
	Texture* m_texture;
	float m_timer;
	float frameSpeed;
};