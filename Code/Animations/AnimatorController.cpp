#include "AnimatorController.h"
#include <iostream>

AnimatorController::AnimatorController(const std::filesystem::path& path)
	: Asset(path, AssetType::AnimatorController)
    , m_defaultState(nullptr)
    , m_anyState(new AnimatorState("Any State"))
{
    AnimationSerializer::Deserialize(*this, path);
}

AnimatorController::~AnimatorController()
{
    delete m_anyState;

    for (auto& state : m_states)
        delete state;
}

void AnimatorController::AddState(AnimatorState* state)
{
    state->SetController(this);
    m_states.push_back(state);
}

void AnimatorController::RemoveState(AnimatorState* state)
{
    for (auto it = m_states.begin(); it != m_states.end(); ++it)
    {
        if (*it == state)
        {
			delete* it;
			m_states.erase(it);

			break;
		}
	}
}

void AnimatorController::AddState(const std::string& name, AnimatorState state)
{
    //m_states.insert({ name, state });
}

void AnimatorController::AddTransition(const std::string& name, AnimatorTransition transition)
{
    /*
    if (m_transitions.find(name) == m_transitions.end())
        m_transitions.insert({ name, {} });
    
    m_transitions[name].push_back(transition);
    */
}

void AnimatorController::Process()
{
    /*
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
        */
}

bool AnimatorController::ChangeParameterName(std::string oldName, std::string newName)
{
    if (m_paramMap.find(oldName) != m_paramMap.end() && m_paramMap.find(newName) == m_paramMap.end())
    {
        m_paramMap[oldName]->name = newName;
        m_paramMap[newName] = m_paramMap[oldName];

        m_paramMap.erase(oldName);

        return true;
    }

    return false;
}

void AnimatorController::AddParameter(const std::string& name, ParameterType value)
{
    if (m_paramMap.find(name) == m_paramMap.end())
    {
        m_parameters.push_back({ name, value });
        m_paramMap[name] = &m_parameters[m_parameters.size() - 1];
    }
}

void AnimatorController::RemoveParameter(const std::string& name)
{
    if (m_paramMap.find(name) != m_paramMap.end())
    {
        for (auto param = m_parameters.begin(); param != m_parameters.end(); ++param)
        {
            if (param->name == name)
            {
				m_parameters.erase(param);
				break;
			}
		}

		m_paramMap.erase(name);
    }
}

void AnimatorController::SetInt(std::string name, int value)
{
    AddParameter(name, value);
}

int AnimatorController::GetInt(std::string name)
{
    return std::get<int>(m_paramMap[name]->value);
}

void AnimatorController::SetBool(std::string name, bool value)
{
    AddParameter(name, value);
}

bool AnimatorController::GetBool(std::string name)
{
    return std::get<bool>(m_paramMap[name]->value);
}
