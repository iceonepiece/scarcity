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

	virtual void Update(float deltaTime) override
	{
		auto spriteView2 = m_registry.view<SpriteAnimatorComponent>();

		for (auto [entity, animator] : spriteView2.each())
			animator.fsm->Process();
	}

	virtual void Render() override
	{
		Renderer& renderer = m_scene->m_app->GetRenderer();


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		
		auto view1 = m_registry.view<TransformComponent,SpriteAnimatorComponent>();
		for (auto [entity, transform, animator] : view1.each())
		{
			if (animator.fsm == nullptr)
				continue;

			AnimationState* animState = (AnimationState*)animator.fsm->GetCurrentState();

			if (animState != nullptr)
			{
				SpriteAnimation& spriteAnim = animState->GetSpriteAnimation();

				//SpriteAnimation *spriteAnim = animator.animations[animator.currentAnimationName];
				//int x = spriteAnim.positions[spriteAnim.currentFrame].x;
				//int y = spriteAnim.positions[spriteAnim.currentFrame].y;

				int x = spriteAnim.currentFrame % spriteAnim.cols;
				int y = (spriteAnim.rows - 1 ) - spriteAnim.currentFrame / spriteAnim.cols;

				float u = spriteAnim.texture.GetWidth() / spriteAnim.cols;
				float v = spriteAnim.texture.GetHeight() / spriteAnim.rows;

				glm::vec2 grid{ x, y };
				//Sprite sprite(&(spriteAnim.texture), spriteAnim.size, grid);
				Sprite sprite(&(spriteAnim.texture), {u, v}, grid);
				glm::vec2 position = transform.position;
				//Renderer::DrawSprite(sprite, glm::vec2{ position.x, position.y }, glm::vec2{ spriteAnim.scale, spriteAnim.scale }, m_camera);

				glm::mat4 model{ 1 };
				model = glm::translate(model, glm::vec3{ position.x, position.y, 0 });

				glm::vec2 ratio = sprite.GetRatio();
				//model = glm::scale(model, glm::vec3{ spriteAnim.scale * ratio.x * transform.scale.x, spriteAnim.scale * ratio.y, 1 });
				model = glm::scale(model, glm::vec3{ ratio.x * transform.scale.x * spriteAnim.scale.x, ratio.y * transform.scale.y * spriteAnim.scale.y, 1 });
				
				renderer.Draw(sprite, model);
			}
		}
		glDisable(GL_BLEND);
		
	}
};