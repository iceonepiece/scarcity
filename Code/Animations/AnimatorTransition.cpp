#include "AnimatorTransition.h"
#include "Animations/AnimatorController.h"
#include "Animations/AnimatorState.h"

AnimatorTransition::AnimatorTransition(AnimatorState* fromState, AnimatorState* nextState)
    : m_fromState(fromState)
    , m_nextState(nextState)
{
    if (m_fromState && m_nextState)
		m_fromState->AddOutgoingTransition(this);

    if (m_nextState)
        m_nextState->AddIncomingTransition(this);
}

AnimatorTransition::~AnimatorTransition()
{
	if (m_fromState)
		m_fromState->RemoveOutgoingTransition(this);

	if (m_nextState)
		m_nextState->RemoveIncomingTransition(this);
}

void AnimatorTransition::AddCondition(const AnimatorCondition& condition)
{
    m_conditions.emplace_back(condition);
}

void AnimatorTransition::RemoveCondition(size_t index)
{
	m_conditions.erase(m_conditions.begin() + index);
}

bool AnimatorTransition::CheckConditions(AnimatorController& fsm)
{
    if (m_nextState == nullptr)
        return false;


    for (auto& condition : m_conditions)
    {
        AnimatorParameter* parameter = fsm.GetParameter(condition.parameter.name);

        if (condition.mode == ConditionMode::True)
        {
            if (std::get<bool>(parameter->value) == false)
                return false;
        }
        else if (condition.mode == ConditionMode::False)
        {
            if (std::get<bool>(parameter->value) == true)
                return false;
        }
    }

    /*
    for (auto& function : m_conditionFunctions)
    {
        if (!function(fsm))
            return false;
    }
    */

    return true;
}

void AnimatorTransition::SetNextState(AnimatorState* state)
{
    m_nextState = state;

}

