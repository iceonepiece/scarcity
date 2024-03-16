#include "AnimatorController.h"
#include <iostream>

AnimatorController::AnimatorController(const std::filesystem::path& path)
	: Asset(path, AssetType::AnimatorController)
    , m_currentState(nullptr)
    , m_defaultState(nullptr)
    , m_anyState(new AnimatorState("Any State"))
{
}

AnimatorController::~AnimatorController()
{
    delete m_anyState;

    for (auto& transition : m_transitions)
        delete transition;

    for (auto& state : m_states)
        delete state;
}

void AnimatorController::DoDeserialize(bool instant)
{
    AnimationSerializer::Deserialize(*this, m_path, instant);
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

void AnimatorController::AddTransition(AnimatorTransition* transition)
{
    m_transitions.push_back(transition);
}

void AnimatorController::AddTransition(const std::string& name, AnimatorTransition transition)
{
    /*
    if (m_transitions.find(name) == m_transitions.end())
        m_transitions.insert({ name, {} });
    
    m_transitions[name].push_back(transition);
    */
}

void AnimatorController::Initialize()
{
    if (m_defaultState != nullptr)
		m_currentState = m_defaultState;
}

void AnimatorController::Process()
{
    if (m_currentState == nullptr)
        return;

    m_currentState->Process(*this);

    std::vector<AnimatorTransition*> checkingTransitions;

    if (m_anyState != nullptr)
    {
        auto& anyTransitions = m_anyState->GetOutgoingTransitions();

        for (auto& transition : anyTransitions)
		{
			checkingTransitions.push_back(transition);
		}
    }

    auto& stateTransitions = m_currentState->GetOutgoingTransitions();

    for (auto& transition : stateTransitions)
    {
        checkingTransitions.push_back(transition);
    }

    AnimatorState* nextState = nullptr;

    for (auto& t : checkingTransitions)
    {
        if (t->CheckConditions(*this))
        {
            nextState = t->GetNextState();
            nextState->OnEnter(*this);
            break;
        }
    }

    if (nextState != nullptr)
        m_currentState = nextState;
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

void AnimatorController::SetInt(const std::string& name, int value)
{
    GetParameter(name)->value = value;
}

int AnimatorController::GetInt(const std::string& name)
{
    if (AnimatorParameter* param = GetParameter(name))
		return std::get<int>(param->value);

    return 0;
}

void AnimatorController::SetFloat(const std::string& name, float value)
{
    GetParameter(name)->value = value;
}

float AnimatorController::GetFloat(const std::string& name)
{
    if (AnimatorParameter* param = GetParameter(name))
        return std::get<float>(param->value);

    return 0;
}

void AnimatorController::SetBool(const std::string& name, bool value)
{
    GetParameter(name)->value = value;
}

bool AnimatorController::GetBool(const std::string& name)
{
    if (AnimatorParameter* param = GetParameter(name))
        return std::get<bool>(param->value);

    return false;
}

void AnimatorController::SetTrigger(const std::string& name)
{
    if (AnimatorParameter* param = GetParameter(name))
    {
        Trigger trigger;
        trigger.value = true;

        param->value = trigger;
    }
}
