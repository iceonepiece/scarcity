#pragma once

#include "StateMachine.h"
#include "Entity.h"

class AITransition
{
public:
    virtual bool CheckConditions(Entity entity, StateMachine* machine)
    {
        return false;
    }
};
