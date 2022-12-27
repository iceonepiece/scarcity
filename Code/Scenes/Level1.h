#pragma once

#include "LevelScene.h"
#include "../Components/Collider2DComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Physics/PlayerFixtureData.h"
#include "../UIs/UIText.h"
#include "../UIs/UIList.h"
#include "../UIs/UIListItem.h"
#include "../UIs/UIContainer.h"

class Level1 : public LevelScene
{
public:
	Level1(Game* game)
		: LevelScene(game)
	{
	}

	virtual void Init() override
	{
		std::cout << "Init Level1" << std::endl;

		auto player = m_manager.CreateEntity();
		b2Body* playerBody = m_physics.CreateBodyWithFixture(b2Vec2{ 0, 8 }, b2Vec2{ 0.5, 1.2 }, new PlayerFixtureData(player), true);
		player.AddComponent<Collider2DComponent>(playerBody);
		player.AddComponent<PlayerComponent>();

		auto platform = m_manager.CreateEntity();
		//platform.AddComponent<Collider2DComponent>(m_physics.CreateBoxBody(0, 0, 15, 0.5));
		platform.AddComponent<Collider2DComponent>(m_physics.CreateBodyWithFixture(b2Vec2{ 0, 0 }, b2Vec2{ 15, 0.5 }, new FixtureData(platform, "PLATFORM")));

		auto platform2 = m_manager.CreateEntity();
		//platform2.AddComponent<Collider2DComponent>(m_physics.CreateBoxBody(-10, -3, 10, 0.5));
		platform2.AddComponent<Collider2DComponent>(m_physics.CreateBodyWithFixture(b2Vec2{ -10, -3 }, b2Vec2{ 10, 0.5 }, new FixtureData(platform2, "PLATFORM")));

		auto platform3 = m_manager.CreateEntity();
		//platform2.AddComponent<Collider2DComponent>(m_physics.CreateBoxBody(-10, -3, 10, 0.5));
		platform3.AddComponent<Collider2DComponent>(m_physics.CreateStaticBox(b2Vec2{ 10, 3 }, b2Vec2{ 10, 0.5 }, glm::radians(15.0f), new FixtureData(platform3, "PLATFORM")));

		m_camera.SetBody(player.GetComponent<Collider2DComponent>()->body);

		auto enemy = m_manager.CreateEntity();
		b2Body* enemyBody = m_physics.CreateBodyWithFixture(b2Vec2{ 1, 8 }, b2Vec2{ 0.5, 1.5 }, new FixtureData(enemy, "ENEMY"), true);
		enemy.AddComponent<Collider2DComponent>(enemyBody);
		enemy.AddComponent<AIComponent>(enemy);

		UIContainer* pausedMenuModal = new UIContainer(glm::vec2(0, 0), glm::vec2(400, 240), glm::vec4(1, 1, 1, 0.5));
		pausedMenuModal->SetAligment(UIAlignment::CENTER);
		m_ui.AddComponent(pausedMenuModal);

		PlayerSystem* playerSystem = new PlayerSystem(this);
		m_systems.emplace_back(playerSystem);

		m_gameStates.emplace("running", new RunningState(this));
		m_gameStates.emplace("paused", new PausedState(this, playerSystem, pausedMenuModal));

		ChangeGameState("running");
	}
};