#include "UIList.h"
#include "UIListItem.h"
#include "../Core/Scene.h"
#include "../Input/Input.h"
#include "../Core/Game.h"

UIList::UIList(glm::vec2 position, glm::vec2 scale, glm::vec4 color)
	: UIComponent(position)
	, scale(scale)
	, color(color)
	, m_selectedIndex(0)
{
}

UIList::~UIList()
{
	for (auto item : m_items)
	{
		delete item;
	}
	m_items.clear();
}

void UIList::AddItem(UIListItem* item)
{
	item->parent = this;
	item->position = position;
	item->position.y = position.y - (m_items.size() * (scale.y + scale.y));

	m_items.push_back(item);
}

void UIList::Update(float deltaTime)
{
	if (Input::IsKeyPressed(Key::Enter))
	{
		m_items[m_selectedIndex]->Enter();
	}
	else if (Input::IsKeyPressed(Key::Up) && m_selectedIndex > 0)
	{
		m_selectedIndex--;
	}
	else if (Input::IsKeyPressed(Key::Down) && m_selectedIndex < m_items.size() - 1)
	{
		m_selectedIndex++;
	}
}

void UIList::Draw()
{
	//Renderer::DrawQuadUI(position, scale, color);

	for (int i = 0; i < m_items.size(); i++)
	{
		m_items[i]->Draw(i == m_selectedIndex);
	}
}