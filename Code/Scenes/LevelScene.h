#pragma once

#include "../Core/Scene.h"
#include "../Components/AIComponent.h"
#include "../Systems/PlayerSystem.h"
#include "../Core/GameState.h"
#include "../Game/RunningState.h"
#include "../Game/PausedState.h"
#include "../Components/CircleCollider2DComponent.h"
#include "../Components/SpriteAnimationComponent.h"
#include "../Game/Car.h"

class LevelScene : public Scene
{
public:
	LevelScene(Game* game)
		: Scene(game)
		, m_car(&(m_physics.m_world))
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

		auto spriteView = m_manager.m_registry.view<SpriteAnimationComponent>();
		for (auto [entity, sprite] : spriteView.each())
		{
			sprite.timer += deltaTime;

			if (sprite.timer > sprite.frameSpeed)
			{
				sprite.currentFrame += 1;

				if (sprite.currentFrame == sprite.positions.size())
					sprite.currentFrame = 0;

				sprite.timer = 0;
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

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		auto view1 = m_manager.m_registry.view<Collider2DComponent, SpriteAnimationComponent>();
		for (auto [entity, collider, spriteAnim] : view1.each())
		{
			int x = spriteAnim.positions[spriteAnim.currentFrame].x;
			int y = spriteAnim.positions[spriteAnim.currentFrame].y;

			glm::vec2 grid{ x, y };

			Sprite sprite(&(spriteAnim.texture), spriteAnim.size, grid);

			b2Vec2 position = collider.body->GetPosition();

			Renderer::DrawSprite(sprite, glm::vec2{ position.x, position.y }, glm::vec2{ spriteAnim.scale, spriteAnim.scale }, m_camera);
		}

		glDisable(GL_BLEND);

		auto view2 = m_manager.m_registry.view<CircleCollider2DComponent>();
		for (auto [entity, collider] : view2.each())
		{
			Renderer::DrawCircle(collider.body, glm::vec4(1, 1, 1, 1), m_camera);
		}

		m_car.Render(m_camera);

		ParticleSystem::Render(m_camera);

		//Renderer::DrawCircle(glm::vec2(-8, 0), 4, glm::vec4(0.7, 0.2, 0.5, 1), m_camera);
	}

protected:
	Car m_car;
};