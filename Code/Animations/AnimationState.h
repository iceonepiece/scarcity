#pragma once

#include "../Core/FSMState.h"
#include "../Core/SpriteAnimation.h"
#include "../Core/Timer.h"

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

			if (m_spriteAnimation.currentFrame == m_spriteAnimation.positions.size())
			{
				m_done = true;
				m_spriteAnimation.currentFrame = 0;
			}

			m_spriteAnimation.timer = 0;
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