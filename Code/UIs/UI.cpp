#include "UI.h"
#include "../Core/Scene.h"
#include "UIComponent.h"

UI::UI(Scene* scene)
	: m_scene(scene)
	, defaultScreenWidth(1280)
	, defaultScreenHeight(720)
{
}

UI::~UI()
{
	for (auto component : m_components)
	{
		delete component;
	}
	m_components.clear();
}

void UI::AddComponent(UIComponent* component)
{
	component->scene = this->m_scene;
	m_components.push_back(component);
}

void UI::Init()
{

}

void UI::ProcessInput(Input& input)
{

}

void UI::Update(float deltaTime)
{
	for (auto component : m_components)
	{
		if (component->IsActive())
			component->Update(deltaTime);
	}
}

void UI::Render()
{
	for (auto component : m_components)
	{
		if (component->IsActive())
			component->Draw();
	}
}

glm::vec2 UI::GetScreenSizePercentage(int width, int height)
{
	float widthPercentage = width / (float)defaultScreenWidth;
	float heightPercentage = height / (float)defaultScreenHeight;

	return glm::vec2(widthPercentage, heightPercentage);
}