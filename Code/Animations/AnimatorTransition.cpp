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
    for (auto& function : m_conditionFunctions)
    {
        if (!function(fsm))
            return false;
    }

    return true;
}

void AnimatorTransition::SetNextState(AnimatorState* state)
{
    m_nextState = state;

}

