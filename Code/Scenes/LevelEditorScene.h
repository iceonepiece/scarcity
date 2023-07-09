#pragma once

#include "../Core/Scene.h"
#include "../Graphics/Renderer.h"
#include "../Components/Collider2DComponent.h"

class LevelEditorScene : public Scene
{
public:
	LevelEditorScene(Application& editor)
	{}

	virtual void Initialize() override
	{
		auto platform = m_manager.CreateEntity();
		b2Body* body = m_physics.CreateBodyWithFixture(b2Vec2{ 0, 0 }, b2Vec2{ 2, 2 }, new FixtureData(platform, "PLATFORM"));
		platform.AddComponent<Collider2DComponent>(body);

		auto platform2 = m_manager.CreateEntity();
		b2Body* body2 = m_physics.CreateBodyWithFixture(b2Vec2{ 3, 3 }, b2Vec2{ 1, 3 }, new FixtureData(platform2, "PLATFORM"));
		platform2.AddComponent<Collider2DComponent>(body2);

		std::cout << "LevelEditorScene.Init()" << std::endl;
	}

	virtual void Render() override
	{
		std::cout << "LevelEditorScene.Render()" << std::endl;
		auto view = m_manager.m_registry.view<Collider2DComponent>();
		for (auto [entity, collider] : view.each())
		{
			std::cout << "LevelEditorScene: DrawQuad" << std::endl;
			//Renderer::DrawQuad(collider.body, *m_camera);
		}
	}
};