#pragma once

#include "../Core/System.h"
#include "../Components/SpriteAnimatorComponent.h"
#include "../Components/Collider2DComponent.h"
#include "../Graphics/RendererAPI.h"
#include "../Animations/AnimationState.h"

class AnimationSystem : public System
{
public:
	AnimationSystem(Scene* scene)
		: System(scene)
	{

	}

	virtual void ProcessInput() override 
	{
		auto spriteView = m_registry.view<PlayerComponent, SpriteAnimatorComponent>();
		for (auto [entity, player, animator] : spriteView.each())
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
	}

	virtual void Update(float deltaTime) override
	{
		auto spriteView = m_registry.view<SpriteAnimatorComponent>();
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
	}

	virtual void Render() override
	{
		Renderer& renderer = RendererAPI::GetRenderer();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		auto view1 = m_registry.view<TransformComponent, Collider2DComponent, SpriteAnimatorComponent>();
		for (auto [entity, transform, collider, animator] : view1.each())
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
				model = glm::scale(model, glm::vec3{ spriteAnim.scale * ratio.x * transform.scale.x, spriteAnim.scale * ratio.y, 1 });

				renderer.Draw(sprite, model);
			}
		}

		glDisable(GL_BLEND);
	}
};