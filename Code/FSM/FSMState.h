#pragma once

#include "FiniteStateMachine.h"
#include "Core/Entity.h"
#include "FSMTransition.h"

class FSMState
{
public:
    FSMState() = default;
    virtual ~FSMState();

    void AddTransition(FSMTransition* transition, FSMState* nextState);
    virtual void OnEnter(FiniteStateMachine* stateMachine);
    virtual void Process(Entity entity, FiniteStateMachine* stateMachine);
    virtual void OnExit();

    inline std::vector<FSMTransition*>& GetTransitions() { return m_transitions; }
    inline bool Done() { return done; }

protected:
    std::vector<FSMTransition*> m_transitions;
    bool done = false;
};
