#pragma once

#include <vector>
#include <functional>
#include <string>

class AnimatorController;
class AnimatorState;
struct AnimatorCondition;

using ConditionFunction = std::function<bool(AnimatorController&)>;

class AnimatorTransition
{
public:
    AnimatorTransition(AnimatorState* fromState, AnimatorState* nextState);
    ~AnimatorTransition();

    void AddCondition(const AnimatorCondition& condition);
    void RemoveCondition(size_t index);
    bool CheckConditions(AnimatorController& fsm);

    AnimatorState* GetFromState() { return m_fromState; }
    AnimatorState* GetNextState() { return m_nextState; }
    void SetNextState(AnimatorState* state);

    std::vector<AnimatorCondition>& GetConditions() { return m_conditions; }

protected:
    std::vector<ConditionFunction> m_conditionFunctions;
    std::vector<AnimatorCondition> m_conditions;

    AnimatorState* m_nextState = nullptr;
    AnimatorState* m_fromState = nullptr;

    friend class AnimationSerializer;

};
