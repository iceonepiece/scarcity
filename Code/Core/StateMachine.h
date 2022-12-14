#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "Entity.h"
#include "AIState.h"
#include "AITransition.h"

typedef std::unordered_map<std::string, AITransition*> TransitionMap;

class StateMachine
{
private:
    std::string currentStateName;

    std::unordered_map<std::string, AIState*> m_states;
    std::unordered_map<std::string, TransitionMap> m_transitions;
    
    Entity entity;

    StateMachine(Entity entity)
        : entity(entity)
    {
        currentStateName = "";
    }

    bool IsCurrentStateDone()
    {
        if (m_states[currentStateName] != nullptr)
        {
            return m_states[currentStateName]->Done();
        }

        return false;
    }

    void AddState(std::string name, AIState* state)
    {
        m_states[name] = state;
    }

    void AddTransition(std::string fromState, std::string toState, AITransition* transition)
    {
        std::cout << "Add transition from " << fromState << " To " << toState << std::endl;

        if (m_transitions.find(fromState) == m_transitions.end())
        {
            m_transitions[fromState] = TransitionMap();
        }

        m_transitions[fromState][toState] = transition;
    }

    void ChangeState(std::string name)
    {
        std::cout << "ChangeState to " << name << std::endl;

        if (m_states[currentStateName] != nullptr)
            m_states[currentStateName]->OnExit();

        currentStateName = name;

        if (m_states[currentStateName] != nullptr)
            m_states[currentStateName]->OnEnter(this);
    }

    void Process()
    {
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
};