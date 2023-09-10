#pragma once

#include "FiniteStateMachine.h"
#include "../Core/Entity.h"

class FSMTransition
{
public:
    virtual ~FSMTransition() = default;
    inline void SetNextState(FSMState* nextState) { m_nextState = nextState; }
    inline FSMState* GetNextState() { return m_nextState; }

    virtual bool CheckConditions(Entity entity, FiniteStateMachine* machine) = 0;

protected:
    FSMState *m_nextState;
};
