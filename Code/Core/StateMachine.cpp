#include "StateMachine.h"
#include "AIState.h"
#include "AITransition.h"

StateMachine::StateMachine(Entity entity)
    : entity(entity)
{
    currentStateName = "";
}

bool StateMachine::IsCurrentStateDone()
{
    if (m_states[currentStateName] != nullptr)
    {
        return m_states[currentStateName]->Done();
    }

    return false;
}

void StateMachine::AddState(std::string name, AIState* state)
{
    m_states[name] = state;
}

void StateMachine::AddTransition(std::string fromState, std::string toState, AITransition* transition)
{
    std::cout << "Add transition from " << fromState << " To " << toState << std::endl;

    if (m_transitions.find(fromState) == m_transitions.end())
    {
        m_transitions[fromState] = TransitionMap();
    }

    m_transitions[fromState][toState] = transition;
}

void StateMachine::ChangeState(std::string name)
{
    std::cout << "ChangeState to " << name << std::endl;

    if (m_states[currentStateName] != nullptr)
        m_states[currentStateName]->OnExit();

    currentStateName = name;

    if (m_states[currentStateName] != nullptr)
        m_states[currentStateName]->OnEnter(this);
}

void StateMachine::Process()
{
    std::cout << "StateMachine.Process()" << std::endl;
    if (m_states.find(currentStateName) != m_states.end())
    {
        m_states[currentStateName]->Process(entity, this);

        if (m_transitions.find(currentStateName) == m_transitions.end())
        {
            for (auto t : m_transitions[currentStateName])
            {
                if (t.second->CheckConditions(entity, this))
                {
                    ChangeState(t.first);
                    break;
                }
            }
        }
    }
}
