#pragma once

#include <vector>
#include <functional>
#include <string>

class AnimatorController;
class AnimatorState;

enum class ConditionMode
{
    None,
    True,
    False,
    Greater,
    Less,
    Equals,
    NotEqual
};

struct AnimatorCondition
{
    ConditionMode mode;
    std::string parameter;
    float threshold;
};

using ConditionFunction = std::function<bool(AnimatorController&)>;

class AnimatorTransition
{
public:
    AnimatorTransition(AnimatorState* fromState, AnimatorState* nextState);
    ~AnimatorTransition();

    void AddCondition(ConditionFunction fn);
    bool CheckConditions(AnimatorController& fsm);

    AnimatorState* GetNextState() { return m_nextState; }
    void SetNextState(AnimatorState* state);

protected:
    std::vector<ConditionFunction> m_conditionFunctions;
    std::vector<AnimatorCondition> m_conditions;

    AnimatorState* m_nextState = nullptr;
    AnimatorState* m_fromState = nullptr;

    friend class AnimationSerializer;

};
