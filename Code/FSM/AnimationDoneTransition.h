#pragma once

#include "FSMTransition.h"
#include "Animations/AnimationState.h"

class AnimationDoneTransition : public FSMTransition
{
public:
	virtual bool CheckConditions(Entity entity, FiniteStateMachine* fsm) override
	{
		AnimationState* state = static_cast<AnimationState*>(fsm->GetCurrentState());
		return state != nullptr && state->IsDone();
	}
};