#pragma once

#include "../Core/GameState.h"

class RunningState : public GameState
{
public:
	RunningState(Scene* scene)
		: GameState(scene)
	{

	}

	virtual void Process() override
	{
		if (Input::IsKeyPressed(Key::Q))
		{
			m_scene->ChangeGameState("paused");
		}
	}

	virtual void OnEnter() override
	{
		m_scene->physicsActive = true;
		m_scene->particleActive = true;
		m_scene->m_systems[0]->active = true;
	}
};