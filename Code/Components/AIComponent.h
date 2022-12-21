#pragma once

#include "../AI/StateMachine.h"

struct AIComponent
{
	StateMachine stateMachine;

	AIComponent(Entity entity)
		: stateMachine(entity)
	{

	}
};