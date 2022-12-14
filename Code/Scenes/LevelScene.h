#pragma once

#include "../Core/Scene.h"
#include "../Components/AIComponent.h"

class LevelScene : public Scene
{
public:
	LevelScene(Game* game)
		: Scene(game)
	{

	}

	virtual void Init() override
	{
		auto player = m_manager.CreateEntity();
		b2Body* playerBody = m_physics.CreateBodyWithFixture(b2Vec2{ 0, 8 }, b2Vec2{ 0.5, 1.5 }, new PlayerFixtureData(player), true);
		player.AddComponent<Collider2DComponent>(playerBody);
		player.AddComponent<PlayerComponent>();

		auto platform = m_manager.CreateEntity();
		platform.AddComponent<Collider2DComponent>(m_physics.CreateBoxBody(0, 0, 15, 0.5));

		auto platform2 = m_manager.CreateEntity();
		platform2.AddComponent<Collider2DComponent>(m_physics.CreateBoxBody(-10, -3, 10, 0.5));

		m_camera.SetBody(player.GetComponent<Collider2DComponent>()->body);

		auto enemy = m_manager.CreateEntity();
		b2Body* enemyBody = m_physics.CreateBodyWithFixture(b2Vec2{ 1, 8 }, b2Vec2{ 0.5, 1.5 }, new FixtureData(enemy, "ENEMY"), true);
		enemy.AddComponent<Collider2DComponent>(enemyBody);
		enemy.AddComponent<AIComponent>(enemy);
	}

	virtual void ProcessInput(Input& input) override
	{
		PlayerSystem::ProcessInput(input, m_manager.m_registry);

		if (input.IsKeyPressed(GLFW_KEY_F))
		{
			ParticleProps props = m_game->GetParticleProps("fire");
			props.position = { 0, 0 };
			for (int i = 0; i < props.amount; i++)
				ParticleSystem::Emit(props);
		}
	}

	virtual void Update(float deltaTime) override
	{
		PlayerSystem::Update(deltaTime, m_manager.m_registry, m_game);

		auto agent = m_manager.m_registry.view<AIComponent>();
		for (auto [entity, ai] : agent.each())
		{
			ai.stateMachine.Process();
		}

		ParticleSystem::Update(deltaTime);
		m_physics.Update(deltaTime);
		m_camera.Update();
	}

	virtual void Render() override
	{
		auto view = m_manager.m_registry.view<Collider2DComponent>();
		for (auto [entity, collider] : view.each())
		{
			Renderer::DrawQuad(collider.body, m_camera);
		}

		ParticleSystem::Render(m_camera);
	}
};