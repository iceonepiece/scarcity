#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <variant>
#include <functional>
#include "Asset/Asset.h"
#include "AnimatorState.h"
#include "AnimatorTransition.h"

struct Trigger {};

using ParameterType = std::variant<float, int, bool, Trigger>;

struct AnimatorParameter
{
    std::string name;
    ParameterType value;
};

enum class ConditionMode
{
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
    AnimatorParameter parameter;
};

class AnimatorController : public Asset
{
public:
    AnimatorController(const std::filesystem::path& path = "");
    virtual ~AnimatorController();

    void AddState(AnimatorState* state);
    void RemoveState(AnimatorState* state);

    void AddState(const std::string& name, AnimatorState state);
    void AddTransition(const std::string& name, AnimatorTransition transition);
    void Process();

    AnimatorState& GetAnyState() { return *m_anyState; }



    inline AnimatorState* GetCurrentState()
    {
        //if (m_states.find(m_currentStateName) != m_states.end())
          //  return &m_states[m_currentStateName];

        return m_currentState;
    }

    inline void SetCurrentStateName(const std::string& name) { m_currentStateName = name; }

    void SetInt(std::string name, int value);
    int GetInt(std::string name);

    void SetBool(std::string name, bool value);
    bool GetBool(std::string name);

    bool ChangeParameterName(std::string oldName, std::string newName);

    void AddParameter(const std::string& name, ParameterType value)
    {
        if (m_paramMap.find(name) == m_paramMap.end())
        {
            m_parameters.push_back({ name, value });
            m_paramMap[name] = m_parameters.size() - 1;
        }
    }

    std::vector<AnimatorState*>& GetStates()
	{
		return m_states;
	}

    std::vector<AnimatorParameter>& GetParameters()
    {
        return m_parameters;
    }

private:
    std::vector<AnimatorParameter> m_parameters;
    std::unordered_map<std::string, size_t> m_paramMap;

    std::vector<AnimatorState*> m_states;

    AnimatorState* m_currentState;
    AnimatorState* m_defaultState;
    AnimatorState* m_anyState = nullptr;

    std::string m_currentStateName;

    /*
    std::string m_defaultStateName;
    std::string m_currentStateName;

    std::vector<AnimatorParameter> m_parameters;
    std::unordered_map<std::string, size_t> m_paramMap;
    std::unordered_map<std::string, AnimatorState> m_states;

    std::vector<AnimatorTransition> m_anyStateTransitions;
    */

    friend class AnimationSerializer;
};