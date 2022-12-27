#pragma once

#include "../Core/GameState.h"
#include "../UIs/UIContainer.h"

class PausedState : public GameState
{
public:

	PausedState(Scene* scene, PlayerSystem* playerSystem, UIContainer* pausedMenuModal)
		: GameState(scene)
		, m_playerSystem(playerSystem)
		, m_pausedMenuModal(pausedMenuModal)
	{
		m_pausedMenuModal->SetActive(false);
	}

	virtual void Process() override
	{
		if (Input::IsKeyPressed(Key::Q))
		{
			m_scene->ChangeGameState("running");
		}
	}

	virtual void OnEnter() override
	{
		m_scene->physicsActive = false;
		m_scene->particleActive = false;
		m_playerSystem->active = false;
		m_pausedMenuModal->SetActive(true);
	}

	virtual void OnExit() override
	{
		m_pausedMenuModal->SetActive(false);
	}

private:
	PlayerSystem* m_playerSystem;
	UIContainer* m_pausedMenuModal;

};