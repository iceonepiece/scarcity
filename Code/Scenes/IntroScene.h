#pragma once

#include "../Core/Scene.h"
#include "../Components/AIComponent.h"
#include "../Core/System.h"
#include "../Core/GameState.h"
#include "../Game/MenuState.h"
#include "../UIs/UIListItem.h"
#include "../UIs/UIList.h"
#include "../UIs/UIText.h"

class IntroScene : public Scene
{
public:
	IntroScene(Application* game)
		: Scene(game)
	{
		m_gameStates.emplace("menu", new MenuState(this));

		ChangeGameState("menu");
	}

	virtual void Init() override
	{
		m_ui.AddComponent(new UIText(glm::vec2(0, 0), "Chapter 1", 1.5f, glm::vec3(1.f, 1.f, 1.f), UIAlignment::CENTER));
	}

	virtual void ProcessInput() override
	{
		for (System* system : m_systems)
		{
			if (system->active)
			{
				system->ProcessInput();
			}
		}
	}

	virtual void Update(float deltaTime) override
	{
		if (m_gameStates[m_currentGameStateName])
		{
			m_gameStates[m_currentGameStateName]->Process();
		}

		for (System* system : m_systems)
		{
			if (system->active)
			{
				system->Update(deltaTime);
			}
		}

		m_ui.Update(deltaTime);
	}

	virtual void Render() override
	{

	}
};