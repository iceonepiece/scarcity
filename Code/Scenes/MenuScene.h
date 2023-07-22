#pragma once

#include "Scene/Scene.h"
#include "../Components/AIComponent.h"
#include "../Core/System.h"
#include "../Core/GameState.h"
#include "../Game/MenuState.h"
#include "../UIs/UIListItem.h"
#include "../UIs/UIList.h"
#include "../UIs/UIText.h"

typedef void(*ActionFn)();

class MenuScene : public Scene
{
public:
	MenuScene(Application& game)
	{
		m_gameStates.emplace("menu", new MenuState(this));

		ChangeGameState("menu");
	}

	virtual void Initialize() override
	{
		m_ui.AddComponent(new UIText(glm::vec2(10, 620), "Ghost Lumber", 1.0f, glm::vec3(1.f, 1.f, 1.f)));
		UIList* list = new UIList(glm::vec2(150, 520), glm::vec2(240, 40), glm::vec4(0.188f, 0.278f, 0.369f, 0.2f));

		UIListItem* startItem = new UIListItem(list->position, "Start Game", glm::vec2(5, 5));
		startItem->actionFunction = ([&]() { m_app->ChangeScene("level1");  });
		list->AddItem(startItem);

		list->AddItem(new UIListItem(list->position, "Options", glm::vec2(5, 5)));
		
		UIListItem* exitItem = new UIListItem(list->position, "Exit Game", glm::vec2(5, 5));
		exitItem->actionFunction = ([]() { std::cout << "DO Exit" << std::endl; });
		list->AddItem(exitItem);
		
		m_ui.AddComponent(list);
		m_ui.AddComponent(new UIText(glm::vec2(0, 0), "Mental Breakdance", 1.0f, glm::vec3(1.f, 1.f, 1.f), UIAlignment::CENTER));
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