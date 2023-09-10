#include "AnimatorController.h"
#include <iostream>

void AnimatorController::AddState(const std::string& name, AnimatorState state)
{
    m_states.insert({ name, state });
}

void AnimatorController::AddTransition(const std::string& name, AnimatorTransition transition)
{
    if (m_transitions.find(name) == m_transitions.end())
        m_transitions.insert({ name, {} });
    
    m_transitions[name].push_back(transition);
}

void AnimatorController::Process()
{
    if (m_states.find(m_currentStateName) == m_states.end())
        return;

    std::vector<AnimatorTransition> checkingTransitions = m_anyStateTransitions;
  
    if (m_transitions.find(m_currentStateName) != m_transitions.end())
        checkingTransitions.insert(checkingTransitions.end(), m_transitions[m_currentStateName].begin(), m_transitions[m_currentStateName].end());

    for (auto& t : checkingTransitions)
    {
        if (t.CheckConditions(*this))
        {
            m_currentStateName = t.GetNextStateName();
            break;
        }
    }

    if (m_states.find(m_currentStateName) != m_states.end())
        m_states[m_currentStateName].Process(*this);
}

void AnimatorController::SetInt(std::string name, int value)
{
    m_parameters[name] = value;
}

int AnimatorController::GetInt(std::string name)
{
    return std::get<int>(m_parameters[name]);
}

void AnimatorController::SetBool(std::string name, bool value)
{
    m_parameters[name] = value;
}

bool AnimatorController::GetBool(std::string name)
{
    return std::get<bool>(m_parameters[name]);
}
