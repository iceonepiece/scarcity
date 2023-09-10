#pragma once

#include <type_traits>
#include "FSM/FSMTransition.h"

template<typename T>
class EqualTransition : public FSMTransition
{
public:
	EqualTransition(std::string value, T target)
		: m_value(value)
		, m_target(target)
	{}

	virtual bool CheckConditions(Entity entity, FiniteStateMachine* fsm)
	{
		if (std::is_same<T, int>::value)
			return fsm->GetInt(m_value) == m_target;
		else if (std::is_same<T, bool>::value)
			return fsm->GetBool(m_value) == m_target;
	}

protected:
	std::string m_value;
	T m_target;
};