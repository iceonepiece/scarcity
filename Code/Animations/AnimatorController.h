#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <functional>
#include "AnimatorState.h"
#include "AnimatorTransition.h"

struct Trigger {};

using ParameterType = std::variant<float, int, bool, Trigger>;

class AnimatorController
{
public:
    void AddState(const std::string& name, AnimatorState state);
    void AddTransition(const std::string& name, AnimatorTransition transition);
    void Process();

    inline AnimatorState* GetCurrentState()
    {
        if (m_states.find(m_currentStateName) != m_states.end())
            return &m_states[m_currentStateName];

        return nullptr;
    }

    inline void SetCurrentStateName(const std::string& name) { m_currentStateName = name; }

    void SetInt(std::string name, int value);
    int GetInt(std::string name);

    void SetBool(std::string name, bool value);
    bool GetBool(std::string name);

    std::unordered_map<std::string, ParameterType>& GetParameters()
    {
        return m_parameters;
    }

private:
    std::string m_currentStateName;

    std::unordered_map<std::string, ParameterType> m_parameters;
    std::unordered_map<std::string, AnimatorState> m_states;

    std::vector<AnimatorTransition> m_anyStateTransitions;
    std::unordered_map<std::string, std::vector<AnimatorTransition>> m_transitions;

};