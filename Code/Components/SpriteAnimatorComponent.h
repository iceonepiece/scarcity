#pragma once

#include <vector>
#include "../Core/Sprite.h"
#include "../Core/SpriteAnimation.h"

struct SpriteAnimatorComponent
{
	FiniteStateMachine *fsm;

	SpriteAnimatorComponent(FiniteStateMachine *fsm)
		: fsm(fsm)
	{}

	~SpriteAnimatorComponent()
	{
		delete fsm;
	}
};
