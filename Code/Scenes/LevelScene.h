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
	}

	virtual void ProcessInput() override
	{
		for (System* system : m_systems)
		{
			if (system->active)
			{
				system->ProcessInput(m_manager.m_registry);
			}
		}


		/*
		if (Input::IsKeyPressed(GLFW_KEY_F))
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

		m_ui.Update(deltaTime);

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

		Renderer::DrawCircle(glm::vec2(-8, 0), 4, glm::vec4(0.7, 0.2, 0.5, 1), m_camera);
	}
};