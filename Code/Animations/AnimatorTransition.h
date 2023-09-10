#pragma once

#include <vector>
#include <functional>
#include <string>

class AnimatorController;

using ConditionFunction = std::function<bool(AnimatorController&)>;

class AnimatorTransition
{
public:
    virtual ~AnimatorTransition() = default;

    void AddCondition(ConditionFunction fn);
    bool CheckConditions(AnimatorController& fsm);

    inline std::string GetNextStateName() { return m_nextStateName; }
    inline void SetNextStateName(const std::string& name) { m_nextStateName = name; }

protected:
    std::vector<ConditionFunction> m_conditions;
    std::string m_nextStateName;

};
