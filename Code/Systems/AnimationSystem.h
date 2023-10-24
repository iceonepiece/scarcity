#pragma once

#include "../Core/System.h"
#include "../Components/SpriteAnimatorComponent.h"
#include "../Components/Collider2DComponent.h"
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
		{
			if (animator.controller == nullptr)
				continue;

			animator.controller->Process();
		}
	}

	virtual void Render() override
	{
		/*
		Renderer& renderer = m_scene->m_app->GetRenderer();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		auto view1 = m_registry.view<TransformComponent, SpriteAnimatorComponent>();
		for (auto [entity, transform, animator] : view1.each())
		{
			if (animator.controller == nullptr)
				continue;

			if (AnimatorState* animState = animator.controller->GetCurrentState())
			{
				SpriteAnimation& spriteAnim = animState->GetSpriteAnimation();
				Sprite sprite = spriteAnim.GetSprite();

				glm::vec2 position = transform.position;
				glm::vec2 ratio = sprite.GetRatio();
				glm::mat4 model{ 1 };
				model = glm::translate(model, glm::vec3{ position.x, position.y, 0 });
				model = glm::scale(model, glm::vec3{ ratio.x * transform.scale.x * spriteAnim.scale.x, ratio.y * transform.scale.y * spriteAnim.scale.y, 1 });

				renderer.Draw(sprite, model);
			}
		}

		glDisable(GL_BLEND);
		*/
	}
};