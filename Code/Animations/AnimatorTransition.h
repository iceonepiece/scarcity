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
    virtual ~AnimatorTransition() = default;

    void AddCondition(ConditionFunction fn);
    bool CheckConditions(AnimatorController& fsm);

    inline std::string GetNextStateName() { return m_nextStateName; }
    inline void SetNextStateName(const std::string& name) { m_nextStateName = name; }
    AnimatorState* GetNextState() { return m_nextState; }
    void SetNextState(AnimatorState* state) { m_nextState = state; }

protected:
    std::vector<ConditionFunction> m_conditionFunctions;
    std::vector<AnimatorCondition> m_conditions;
    std::string m_nextStateName;

    AnimatorState* m_nextState = nullptr;

    friend class AnimationSerializer;

};
