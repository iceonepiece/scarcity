#pragma once

#include <functional>
#include <glm/glm.hpp>
#include "UIComponent.h"
#include "UIText.h"

using ActionFunction = std::function<void()>;

class UIListItem : public UIComponent
{
public:
	UIListItem(glm::vec2 position, std::string text, glm::vec2 padding = glm::vec2(0, 0));
	~UIListItem();

	virtual void Draw() override;
	void Draw(bool active);
	void Enter();

	ActionFunction actionFunction;

	UIText* uiText;
	glm::vec2 padding;
};