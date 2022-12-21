#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "../Core/Entity.h"

class AIState;
class AITransition;

typedef std::unordered_map<std::string, AITransition*> TransitionMap;

class StateMachine
{
private:
    std::string currentStateName;

    std::unordered_map<std::string, AIState*> m_states;
    std::unordered_map<std::string, TransitionMap> m_transitions;

    Entity entity;

public:
    StateMachine(Entity entity);
    bool IsCurrentStateDone();

    void AddState(std::string name, AIState* state);
    void AddTransition(std::string fromState, std::string toState, AITransition* transition);
    void ChangeState(std::string name);
    void Process();
};