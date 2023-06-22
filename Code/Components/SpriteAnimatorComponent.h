#pragma once

#include <vector>
#include "../Animations/Sprite.h"
#include "../Animations/SpriteAnimation.h"

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
