#pragma once

#include <vector>
#include <string>
#include "UIComponent.h"

class UIListItem;

class UIList : public UIComponent
{
public:
	UIList(glm::vec2 position, glm::vec2 scale, glm::vec4 color);
	~UIList();

	void AddItem(UIListItem* item);
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	glm::vec2 scale;
	glm::vec4 color;

	std::vector<UIListItem*> m_items;
	unsigned int m_selectedIndex = -1;
};