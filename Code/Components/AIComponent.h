#pragma once

#include "FSM/FiniteStateMachine.h"

struct AIComponent
{
	FiniteStateMachine stateMachine;

	AIComponent(Entity entity)
		: stateMachine(entity)
	{

	}
};