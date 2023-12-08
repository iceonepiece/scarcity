#include "AnimatorTransition.h"
#include "Animations/AnimatorController.h"
#include "Animations/AnimatorState.h"

AnimatorTransition::AnimatorTransition(AnimatorState* fromState, AnimatorState* nextState)
    : m_fromState(fromState)
    , m_nextState(nextState)
    , m_hasExitTime(false)
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
    if (m_nextState == nullptr || fsm.GetCurrentState() == nullptr)
        return false;

    if (m_hasExitTime && fsm.GetCurrentState()->IsDone())
        return true;

    for (auto& condition : m_conditions)
    {
        AnimatorParameter* parameter = fsm.GetParameter(condition.parameter.name);

        if (std::holds_alternative<int>(parameter->value))
        {
            if (condition.mode == ConditionMode::Greater)
            {
                if (std::get<int>(parameter->value) <= std::get<int>(condition.parameter.value))
                    return false;
            }
            else if (condition.mode == ConditionMode::Less)
			{
				if (std::get<int>(parameter->value) >= std::get<int>(condition.parameter.value))
					return false;
			}
			else if (condition.mode == ConditionMode::Equals)
			{
				if (std::get<int>(parameter->value) != std::get<int>(condition.parameter.value))
					return false;
			}
            else if (condition.mode == ConditionMode::NotEqual)
			{
				if (std::get<int>(parameter->value) == std::get<int>(condition.parameter.value))
					return false;
			}
        }
        else if (std::holds_alternative<bool>(parameter->value))
        {
            if (condition.mode == ConditionMode::True)
            {
                return std::get<bool>(parameter->value) == true;
            }
            else if (condition.mode == ConditionMode::False)
            {
                return std::get<bool>(parameter->value) == false;
            }
        }
        else if (std::holds_alternative<Trigger>(parameter->value))
        {
            Trigger trigger = std::get<Trigger>(parameter->value);
            if (trigger.value == true)
            {
				trigger.value = false;
                parameter->value = trigger;

				return true;
			}
			else
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

    return false;
}

void AnimatorTransition::SetNextState(AnimatorState* state)
{
    m_nextState = state;

}

