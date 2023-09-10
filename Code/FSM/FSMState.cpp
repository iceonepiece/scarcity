#include "FSMState.h"

FSMState::~FSMState()
{
    std::cout << "~FSMState" << std::endl;
    for (auto transition : m_transitions)
    {
        delete transition;
    }
    m_transitions.clear();
}

void FSMState::AddTransition(FSMTransition* transition, FSMState* nextState)
{
    transition->SetNextState(nextState);
    m_transitions.push_back(transition);
}

void FSMState::OnEnter(FiniteStateMachine* stateMachine)
{
}

void FSMState::Process(Entity entity, FiniteStateMachine* stateMachine)
{
}

void FSMState::OnExit()
{
}
