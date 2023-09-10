#include "FiniteStateMachine.h"
#include "FSMState.h"
#include "FSMTransition.h"

FiniteStateMachine::FiniteStateMachine()
    : m_entity(Entity{})
    , m_currentState(nullptr)
    , m_anyState(new FSMState())
{
}

FiniteStateMachine::FiniteStateMachine(Entity entity)
    : m_entity(entity)
    , m_currentState(nullptr)
    , m_anyState(new FSMState())
{

}

FiniteStateMachine::~FiniteStateMachine()
{
    for (auto& i : m_states)
    {
        delete i.second;
    }
    m_states.clear();

    delete m_anyState;

    for (auto& i : m_values)
    {
        delete i.second;
    }
    m_values.clear();
}

void FiniteStateMachine::SetInt(std::string name, int value)
{
    if (m_values[name] != nullptr)
    {
        IntValue* v = dynamic_cast<IntValue*>(m_values[name]);
        
        if (v != nullptr)
            v->value = value;
    }
}

int FiniteStateMachine::GetInt(std::string name)
{
    if (m_values[name] != nullptr)
    {
        IntValue* v = dynamic_cast<IntValue*>(m_values[name]);

        if (v != nullptr)
            return v->value;
    }

    return -1;
}

bool FiniteStateMachine::IsCurrentStateDone()
{
    return m_currentState != nullptr && m_currentState->Done();
}

void FiniteStateMachine::AddState(std::string name, FSMState* state)
{
    m_states[name] = state;
}

void FiniteStateMachine::SetCurrentState(FSMState *state)
{
    if (m_currentState != nullptr)
        m_currentState->OnExit();

    m_currentState = state;

    if (m_currentState != nullptr)
        m_currentState->OnEnter(this);
}

void FiniteStateMachine::Process()
{

    if (m_currentState != nullptr)
    {
        std::vector<FSMTransition*> transitions = m_anyState->GetTransitions();
        std::vector<FSMTransition*> currentTransitions = m_currentState->GetTransitions();

        transitions.insert(transitions.end(), currentTransitions.begin(), currentTransitions.end());

        for (auto t : transitions)
        {
            if (t->CheckConditions(m_entity, this))
            {
                SetCurrentState(t->GetNextState());
                break;
            }
        }

        m_currentState->Process(m_entity, this);
    }
}

FSMState* FiniteStateMachine::GetCurrentState()
{
    return m_currentState;
}

FSMState* FiniteStateMachine::GetAnyState()
{
    return m_anyState;
}

void FiniteStateMachine::AddValue(std::string name, Value* value)
{
    m_values[name] = value;
}


template<typename T>
T FiniteStateMachine::GetValue(std::string name)
{
    return m_values[name];
}

void FiniteStateMachine::SetBool(std::string name, bool value)
{
    if (m_values[name] != nullptr)
    {
        BoolValue* v = dynamic_cast<BoolValue*>(m_values[name]);

        if (v != nullptr)
            v->value = value;
    }
}

bool FiniteStateMachine::GetBool(std::string name)
{
    if (m_values[name] != nullptr)
    {
        BoolValue* v = dynamic_cast<BoolValue*>(m_values[name]);

        return v->value;
    }

    return false;
}