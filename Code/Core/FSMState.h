#pragma once

#include "FiniteStateMachine.h"
#include "Core/Entity.h"
#include "FSMTransition.h"

class FSMState
{
public:
    virtual ~FSMState()
    {
        std::cout << "~FSMState" << std::endl;
        for (auto transition : m_transitions)
        {
            delete transition;
        }
        m_transitions.clear();
    }

    std::vector<FSMTransition*>& GetTransitions()
    {
        return m_transitions;
    }

    void AddTransition(FSMTransition* transition, FSMState* nextState)
    {
        transition->SetNextState(nextState);
        m_transitions.push_back(transition);
    }

    virtual void OnEnter(FiniteStateMachine* stateMachine)
    {

    }

    virtual void Process(Entity entity, FiniteStateMachine* stateMachine)
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
    std::vector<FSMTransition*> m_transitions;
    bool done = false;
};
