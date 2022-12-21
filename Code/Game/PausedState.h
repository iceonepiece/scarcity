#pragma once

#include "../Core/GameState.h"

class PausedState : public GameState
{
public:
	PausedState(Scene* scene)
		: GameState(scene)
	{

	}

	virtual void Process() override
	{
		Input& input = m_scene->m_game->GetInput();

		if (input.IsKeyPressed(GLFW_KEY_Q))
		{
			m_scene->ChangeGameState("running");
		}
	}

	virtual void OnEnter() override
	{
		m_scene->physicsActive = false;
		m_scene->particleActive = false;
		m_scene->m_systems[0]->active = false;
	}
};