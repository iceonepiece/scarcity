#include "EditorScene.h"
#include "../Core/Game.h"
#include "../GUIs/GUIWindow.h"
#include "../Components/Collider2DComponent.h"
#include "../Editor/EntitiesWindow.h"
#include "../Editor/InspectorWindow.h"

EditorScene::EditorScene(GameApplication* game)
	: Scene(game)
	, m_gui(game)
	, m_editorMouse(this)
{

}

EditorScene::~EditorScene()
{
	m_gui.Destroy();
}

void EditorScene::Init()
{
	//m_gui.Init(m_game->GetWindow(), "#version 330");

	EntitiesWindow* entitiesWindow = new EntitiesWindow(&m_gui, this);
	m_gui.AddComponent(entitiesWindow);
	m_gui.AddComponent(new InspectorWindow(&m_gui, this, entitiesWindow));

	auto platform = m_manager.CreateEntity();
	b2Body* body = m_physics.CreateBodyWithFixture(b2Vec2 { 0, 0 }, b2Vec2 { 2, 2 }, new FixtureData(platform, "PLATFORM"));
	platform.AddComponent<Collider2DComponent>(body);

	auto platform2 = m_manager.CreateEntity();
	b2Body* body2 = m_physics.CreateBodyWithFixture(b2Vec2{ 3, 3 }, b2Vec2{ 1, 3 }, new FixtureData(platform2, "PLATFORM"));
	platform2.AddComponent<Collider2DComponent>(body2);
}

void EditorScene::ProcessInput()
{
	
}

void EditorScene::OnEvent(Event* e)
{
	if (!m_gui.BlockEvent(e))
	{
		m_editorMouse.OnEvent(e);
		//m_editorMouse.Update(m_camera);;
	}
}

void EditorScene::Update(float deltaTime)
{
	//m_editorMouse.Update(m_camera);
}

void EditorScene::Render()
{
	auto view = m_manager.m_registry.view<Collider2DComponent>();
	for (auto [entity, collider] : view.each())
	{
		Renderer::DrawQuad(collider.body, m_camera);
	}

	m_gui.NewFrame();
	m_gui.Draw();
}
