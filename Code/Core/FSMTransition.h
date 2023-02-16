#pragma once

#include "FiniteStateMachine.h"
#include "../Core/Entity.h"

class FSMTransition
{
public:
    virtual ~FSMTransition() = default;

    void SetNextState(FSMState *nextState)
    {
        m_nextState = nextState;
    }

    FSMState* GetNextState()
    {
        return m_nextState;
    }

    virtual bool CheckConditions(Entity entity, FiniteStateMachine* machine) = 0;

protected:
    FSMState *m_nextState;
};
