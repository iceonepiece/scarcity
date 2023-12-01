#include "AnimatorState.h"
#include "AnimatorController.h"
#include "Core/Timer.h"

AnimatorState::AnimatorState(const std::string& name)
	: m_name(name)
	, m_controller(nullptr)
	, m_position(0.0f, 0.0f)
	, m_selected(false)
	, m_motion(nullptr)
	, m_speed(1.0f)
	, m_done(false)
{
}

AnimatorState::~AnimatorState()
{
	for (auto& transition : m_outgoingTransitions)
		delete transition;

	for (auto& transition : m_incomingTransitions)
		delete transition;
}

AnimatorState::AnimatorState(SpriteAnimation spriteAnimation)
	//: m_spriteAnimation(spriteAnimation)
{
}

AnimatorState& AnimatorState::operator=(const AnimatorState& other)
{
	std::cout << "AnimatorState operator=\n";
	//m_spriteAnimation = other.m_spriteAnimation;
	m_done = other.IsDone();

	return *this;
}

void AnimatorState::AddOutgoingTransition(AnimatorTransition* transition)
{
	m_outgoingTransitions.push_back(transition);
}
void AnimatorState::AddIncomingTransition(AnimatorTransition* transition)
{
	m_incomingTransitions.push_back(transition);
}

bool AnimatorState::RemoveOutgoingTransition(AnimatorTransition* transition)
{
	for (auto it = m_outgoingTransitions.begin(); it != m_outgoingTransitions.end(); ++it)
	{
		if (transition == *it)
		{
			m_outgoingTransitions.erase(it);

			return true;
		}
	}

	return false;
}

bool AnimatorState::RemoveTransition(AnimatorState* toState)
{
	for (int i = 0; i < m_outgoingTransitions.size(); ++i)
	{
		if (toState == m_outgoingTransitions[i]->GetNextState())
		{
			delete m_outgoingTransitions[i];

			return true;
		}
	}
}

bool AnimatorState::RemoveIncomingTransition(AnimatorTransition* transition)
{
	for (auto it = m_incomingTransitions.begin(); it != m_incomingTransitions.end(); ++it)
	{
		if (transition == *it)
		{
			m_incomingTransitions.erase(it);

			return true;
		}
	}

	return false;
}

void AnimatorState::Process(AnimatorController& fsm)
{
	std::cout << "AnimatorState::Process: " << m_name << "\n";
	/*
	m_spriteAnimation.timer += Timer::GetDeltaTime();

	if (m_spriteAnimation.timer > m_spriteAnimation.frameSpeed)
	{
		m_spriteAnimation.currentFrame += 1;

		if (m_spriteAnimation.currentFrame == m_spriteAnimation.startFrame + m_spriteAnimation.maxFame)
		{
			m_done = true;
			m_spriteAnimation.currentFrame = m_spriteAnimation.startFrame;
		}

		m_spriteAnimation.timer = 0.0f;
	}
	*/
}