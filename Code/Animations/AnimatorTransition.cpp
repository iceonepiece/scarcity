#include "AnimatorTransition.h"
#include "AnimatorController.h"

void AnimatorTransition::AddCondition(ConditionFunction fn)
{
    m_conditions.emplace_back(fn);
}

bool AnimatorTransition::CheckConditions(AnimatorController& fsm)
{
    for (auto& condition : m_conditions)
    {
        if (!condition(fsm))
            return false;
    }

    return true;
}

