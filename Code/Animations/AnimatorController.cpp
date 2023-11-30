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
    if (m_currentState == nullptr)
        return;

    std::vector<AnimatorTransition*> checkingTransitions = m_anyState->GetOutgoingTransitions();
    std::vector<AnimatorTransition*> stateTransitions = m_currentState->GetOutgoingTransitions();

    checkingTransitions.insert(checkingTransitions.end(), stateTransitions.begin(), stateTransitions.end());

    for (auto& t : checkingTransitions)
    {
        if (t->CheckConditions(*this))
        {
            m_currentState = t->GetNextState();
            break;
        }
    }

    if (m_currentState != nullptr)
        m_currentState->Process(*this);
}

bool AnimatorController::ChangeParameterName(std::string oldName, std::string newName)
{
    if (GetParameter(newName) != nullptr)
    {
		return false;
    }

    if (AnimatorParameter* param = GetParameter(oldName))
    {
        param->name = newName;
        return true;
    }

    return false;
}

void AnimatorController::AddParameter(const std::string& name, ParameterType value)
{
    if (GetParameter(name) != nullptr)
		return;

    m_parameters.push_back({ name, value });
}

void AnimatorController::RemoveParameter(const std::string& name)
{
    for (auto it = m_parameters.begin(); it != m_parameters.end(); ++it)
    {
        if (it->name == name)
        {
			m_parameters.erase(it);
			break;
		}
	}
}

void AnimatorController::SetInt(std::string name, int value)
{
    AddParameter(name, value);
}

int AnimatorController::GetInt(std::string name)
{
    if (AnimatorParameter* param = GetParameter(name))
		return std::get<int>(param->value);

    return 0;
}

void AnimatorController::SetBool(std::string name, bool value)
{
    AddParameter(name, value);
}

bool AnimatorController::GetBool(std::string name)
{
    if (AnimatorParameter* param = GetParameter(name))
        return std::get<bool>(param->value);

    return false;
}
