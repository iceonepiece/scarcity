#include "AnimatorTransition.h"
#include "Animations/AnimatorController.h"
#include "Animations/AnimatorState.h"

void AnimatorTransition::AddCondition(ConditionFunction fn)
{
    m_conditionFunctions.emplace_back(fn);
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

