#include "AnimatorState.h"
#include "AnimatorController.h"
#include "Core/Timer.h"

AnimatorState::AnimatorState(const std::string& name)
	: m_name(name)
	//: m_spriteAnimation(SpriteAnimation())
{
}

AnimatorState::~AnimatorState()
{
	for (auto& transition : m_transitions)
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

void AnimatorState::AddTransition(AnimatorTransition* transition)
{
	m_transitions.push_back(transition);
}

bool AnimatorState::RemoveTransition(AnimatorState* toState)
{
	for (auto it = m_transitions.begin(); it != m_transitions.end(); ++it)
	{
		if (toState == (*it)->GetNextState())
		{
			m_transitions.erase(it);
			return true;
		}
	}

	return false;
}

void AnimatorState::Process(AnimatorController& fsm)
{
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