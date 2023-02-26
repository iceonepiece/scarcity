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

		auto spriteView = m_manager.m_registry.view<SpriteAnimatorComponent>();
		for (auto [entity, animator] : spriteView.each())
		{
			
			if (Input::IsKeyHeld(Key::Left) || Input::IsKeyHeld(Key::Right))
			{
				animator.fsm->SetBool("walking", true);
			}
			else
			{
				animator.fsm->SetBool("walking", false);
			}

			if (Input::IsKeyPressed(Key::A))
			{
				std::cout << "Pressed A" << std::endl;
				animator.fsm->SetInt("attacking", 1);
				//animator.fsm->SetTrigger("attacking", 1);
				//animator.currentAnimationName = "Attack";
			}
			else if (Input::IsKeyPressed(Key::S))
			{
				std::cout << "Pressed S" << std::endl;
				//animator.fsm->SetInt("attacking", 0);
				//animator.currentAnimationName = "Idle";
			}
			else
			{
				animator.fsm->SetInt("attacking", 0);
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

		auto spriteView = m_manager.m_registry.view<SpriteAnimatorComponent>();
		for (auto [entity, animator] : spriteView.each())
		{
			animator.fsm->Process();
			/*
			SpriteAnimation *sprite = animator.animations[animator.currentAnimationName];
			sprite->timer += deltaTime;

			if (sprite->timer > sprite->frameSpeed)
			{
				sprite->currentFrame += 1;

				if (sprite->currentFrame == sprite->positions.size())
					sprite->currentFrame = 0;

				sprite->timer = 0;
			}
			*/
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
			Renderer::DrawQuad(collider.body, m_camera);
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		auto view1 = m_manager.m_registry.view<Collider2DComponent, SpriteAnimatorComponent>();
		for (auto [entity, collider, animator] : view1.each())
		{
			AnimationState* animState = (AnimationState*)animator.fsm->GetCurrentState();

			if (animState != nullptr)
			{
				SpriteAnimation& spriteAnim = animState->GetSpriteAnimation();

				//SpriteAnimation *spriteAnim = animator.animations[animator.currentAnimationName];
				int x = spriteAnim.positions[spriteAnim.currentFrame].x;
				int y = spriteAnim.positions[spriteAnim.currentFrame].y;

				glm::vec2 grid{ x, y };
				Sprite sprite(&(spriteAnim.texture), spriteAnim.size, grid);
				b2Vec2 position = collider.body->GetPosition();
				//Renderer::DrawSprite(sprite, glm::vec2{ position.x, position.y }, glm::vec2{ spriteAnim.scale, spriteAnim.scale }, m_camera);
				
				glm::mat4 model{ 1 };
				model = glm::translate(model, glm::vec3{ position.x, position.y, 0 });

				glm::vec2 ratio = sprite.GetRatio();
				model = glm::scale(model, glm::vec3{ spriteAnim.scale * ratio.x, spriteAnim.scale * ratio.y, 1 });
				
				renderer.Draw(sprite, model, m_camera);
			}
		}

		glDisable(GL_BLEND);

		auto view2 = m_manager.m_registry.view<CircleCollider2DComponent>();
		for (auto [entity, collider] : view2.each())
		{
			Renderer::DrawCircle(collider.body, glm::vec4(1, 1, 1, 1), m_camera);
		}

		renderer.DrawLine(glm::vec3{ -5, -5, 0 }, glm::vec3{5, 5, 0 }, glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));

		m_car.Render(m_camera);

		ParticleSystem::Render(m_camera);

		//Renderer::DrawCircle(glm::vec2(-8, 0), 4, glm::vec4(0.7, 0.2, 0.5, 1), m_camera);
	}

protected:
	Car m_car;
};