#pragma once

#include "../Core/Scene.h"
#include "../Components/AIComponent.h"
#include "../Systems/PlayerSystem.h"
#include "../Core/GameState.h"
#include "../Game/RunningState.h"
#include "../Game/PausedState.h"

class LevelScene : public Scene
{
public:
	LevelScene(Game* game)
		: Scene(game)
	{
		m_systems.emplace_back(new PlayerSystem(this));
		//m_systems.emplace_back(new ParticleSystem(this));
		//m_systems.emplace_back(new CameraSystem(this));

		m_gameStates.emplace("running", new RunningState(this));
		m_gameStates.emplace("paused", new PausedState(this));

		ChangeGameState("running");
	}

	virtual void ProcessInput(Input& input) override
	{
		for (System* system : m_systems)
		{
			if (system->active)
			{
				system->ProcessInput(input, m_manager.m_registry);
			}
		}


		/*
		if (input.IsKeyPressed(GLFW_KEY_F))
		{
			ParticleProps props = m_game->GetParticleProps("fire");
			props.position = { 0, 0 };
			for (int i = 0; i < props.amount; i++)
				ParticleSystem::Emit(props);
		}
		*/
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
				system->Update(deltaTime, m_manager.m_registry);
			}
		}

		if (particleActive)
			ParticleSystem::Update(deltaTime);

		if (physicsActive)
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