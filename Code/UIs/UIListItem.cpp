#include "UIListItem.h"
#include "UIList.h"
#include "../Graphics/Renderer.h"

UIListItem::UIListItem(glm::vec2 position, std::string text, glm::vec2 padding)
	: UIComponent(position)
	, padding(padding)
{
	uiText = new UIText(position, text, 0.8f, glm::vec3(1.0, 1.0, 1.0));
	uiText->scene = scene;
}

UIListItem::~UIListItem()
{
	if (uiText != nullptr)
	{
		delete uiText;
	}
}

void UIListItem::Enter()
{
	if (actionFunction)
		actionFunction();
}

void UIListItem::Draw(bool active)
{
	UIList* parentList = (UIList*)(parent);

	glm::vec4 color = active ? glm::vec4(0.6, 0.6, 0.6, 1.0) : glm::vec4(0.2, 0.2, 0.2, 1.0);
	
	Renderer::DrawQuadUI(position, parentList->scale, color);
	uiText->position.x = position.x - (parentList->scale.x / 2) + padding.x;
	uiText->position.y = position.y - (parentList->scale.y / 2) + padding.y;
	uiText->Draw();
}

void UIListItem::Draw()
{
	UIList* parentList = (UIList*)(parent);
	Renderer::DrawQuadUI(position, parentList->scale, glm::vec4(0.5, 0.5, 0.5, 1.0));
	uiText->position.x = position.x - (parentList->scale.x / 2) + padding.x;
	uiText->position.y = position.y - (parentList->scale.y / 2) + padding.y;
	uiText->Draw();
}