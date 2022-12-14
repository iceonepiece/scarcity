#pragma once

#include "../Core/StateMachine.h"

struct AIComponent
{
	StateMachine stateMachine;

	AIComponent(Entity entity)
		: stateMachine(entity)
	{

	}
};