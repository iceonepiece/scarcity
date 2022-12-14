#pragma once

#include "StateMachine.h"
#include "Entity.h"

class AIState
{
public:

    virtual void OnEnter(StateMachine* stateMachine)
    {

    }

    virtual void Process(Entity entity, StateMachine* stateMachine)
    {

    }

    virtual void OnExit()
    {

    }

    bool Done()
    {
        return done;
    }

protected:
    bool done = false;
};
