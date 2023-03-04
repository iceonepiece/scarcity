#pragma once

#include "../Core/Scene.h"
#include "../Components/AIComponent.h"
#include "../Systems/PlayerSystem.h"
#include "../Core/GameState.h"
#include "../Game/RunningState.h"
#include "../Game/PausedState.h"
#include "../Components/CircleCollider2DComponent.h"
#include "../Components/SpriteAnimatorComponent.h"
#include "../Game/Car.h"
#include "../Animations/AnimationState.h"
#include "../Graphics/RendererAPI.h"

class LevelScene : public Scene
{
public:
	LevelScene(Application* game)
		: Scene(game)
	{
	}

	virtual void ProcessInput() override
	{
		for (System* system : m_systems)
		{
			if (system->active)
				system->ProcessInput();
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
				system->Update(deltaTime);
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

		Renderer& renderer = RendererAPI::GetRenderer();
		renderer.SetCamera(&m_camera);

		auto view = m_manager.m_registry.view<Collider2DComponent>();
		for (auto [entity, collider] : view.each())
		{
			//Renderer::DrawQuad(collider.body, m_camera);
			renderer.DrawRect(collider.body, m_camera);
		}
		
		for (auto system : m_systems)
		{
			if (system->active)
				system->Render();
		}

		auto view2 = m_manager.m_registry.view<CircleCollider2DComponent>();
		for (auto [entity, collider] : view2.each())
		{
			Renderer::DrawCircle(collider.body, glm::vec4(1, 1, 1, 1), m_camera);
		}

		renderer.DrawLine(glm::vec3{ -5, -5, 0 }, glm::vec3{5, 5, 0 }, glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));

		ParticleSystem::Render(m_camera);

		//Renderer::DrawCircle(glm::vec2(-8, 0), 4, glm::vec4(0.7, 0.2, 0.5, 1), m_camera);
	}
};