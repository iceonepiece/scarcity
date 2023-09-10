#pragma once

#include "FSM/FSMState.h"
#include "SpriteAnimation.h"
#include "Core/Timer.h"

class AnimationState : public FSMState
{
public:
	AnimationState(SpriteAnimation spriteAnimation)
		: m_spriteAnimation(spriteAnimation)
	{

	}

	virtual void OnEnter(FiniteStateMachine* stateMachine) override
	{
		m_done = false;
	}

	virtual void Process(Entity entity, FiniteStateMachine* stateMachine) override
	{
		m_spriteAnimation.timer += Timer::GetDeltaTime();

		if (m_spriteAnimation.timer > m_spriteAnimation.frameSpeed)
		{
			m_spriteAnimation.currentFrame += 1;

			//if (m_spriteAnimation.currentFrame == m_spriteAnimation.positions.size())
			if (m_spriteAnimation.currentFrame == m_spriteAnimation.startFrame + m_spriteAnimation.maxFame)
			{
				m_done = true;
				m_spriteAnimation.currentFrame = m_spriteAnimation.startFrame;
			}

			m_spriteAnimation.timer = 0.0f;
		}
	}

	SpriteAnimation& GetSpriteAnimation()
	{
		return m_spriteAnimation;
	}

	bool IsDone()
	{
		return m_done;
	}

protected:
	SpriteAnimation m_spriteAnimation;
	bool m_done = false;
};