#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "../Core/Entity.h"
#include "Value.h"

class FSMState;
class FSMTransition;

class FiniteStateMachine
{
public:
    FiniteStateMachine(Entity entity);
    ~FiniteStateMachine();

    bool IsCurrentStateDone();

    void AddState(std::string name, FSMState* state);
    void SetCurrentState(FSMState *state);
    void Process();

    void AddValue(std::string name, Value* value);

    void SetInt(std::string name, int value);
    int GetInt(std::string name);

    void SetBool(std::string name, bool value);
    bool GetBool(std::string name);

    template<typename T>
    T GetValue(std::string name);

    FSMState* GetCurrentState();

protected:
    FSMState* m_currentState;
    std::unordered_map<std::string, Value*> m_values;
    std::unordered_map<std::string, FSMState*> m_states;
    Entity entity;
};