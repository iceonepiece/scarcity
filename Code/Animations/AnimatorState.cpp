#include "AnimatorState.h"
#include "AnimatorController.h"
#include "Core/Timer.h"

AnimatorState::AnimatorState()
	//: m_spriteAnimation(SpriteAnimation())
{
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