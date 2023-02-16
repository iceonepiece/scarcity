#pragma once

#include "../Core/FiniteStateMachine.h"

struct AIComponent
{
	FiniteStateMachine stateMachine;

	AIComponent(Entity entity)
		: stateMachine(entity)
	{

	}
};