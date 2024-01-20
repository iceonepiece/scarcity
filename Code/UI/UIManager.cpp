#include "UIManager.h"
#include "Math/Math.h"
#include <iostream>

std::vector<UIObject> UIManager::m_objects;

void UIManager::ProcessButton(ButtonComponent& button, Input& input)
{

}

bool UIManager::HandleInput(CanvasComponent& canvas, ButtonComponent& button, Input& input)
{
	if (Math::Contains(canvas.position, canvas.size, input.GetCursorPosition()))
	{
		if (input.GetMouseButtonDown(Mouse::ButtonLeft))
		{
			button.mousePressedHandler.Invoke(&button, button);
		}

		if (!button.mouseOver)
		{
			std::cout << "Mouse Enter\n";
			button.mouseEnterHandler.Invoke(&button, button);
			button.mouseOver = true;
		}
		if (!button.focused && input.GetMouseButtonDown(Mouse::ButtonLeft))
		{
			std::cout << "Focus Received\n";
			//m_onFocusReceived.Invoke(this, *this);
			button.focused = true;
			return true;
		}
	}
	else
	{
		if (button.mouseOver)
		{
			std::cout << "Mouse Leave\n";
			button.mouseLeaveHandler.Invoke(&button, button);
			button.mouseOver = false;
		}

		if (!button.focused && input.GetMouseButtonDown(Mouse::ButtonLeft))
		{
			//m_onFocusLost.Invoke(this, *this);
			button.focused = false;
		}
	}

	return false;
}

void UIManager::CreateUIComponent(sol::table& uiComponent, const UIRect& parentRect)
{
	float x = uiComponent["x"].get<float>();
	float y = uiComponent["y"].get<float>();
	float w = uiComponent["w"].get<float>();
	float h = uiComponent["h"].get<float>();

	glm::vec4 color = {
		uiComponent["color"][1],
		uiComponent["color"][2],
		uiComponent["color"][3],
		uiComponent["color"][4]
	};

	UIObject object;
	object.type = (UIType)uiComponent["class"].get<int>();
	object.position = { x + parentRect.position.x, y + parentRect.position.y, 0 };
	object.scale = { w,  h, 0 };
	object.color = color;
	object.flags = (UIFlag)uiComponent["flags"].get<int>();

	if (object.flags & UIFlag_HorizontalCenter)
	{
		object.position.x += (parentRect.scale.x / 2) + parentRect.position.x;
	}

	if (object.flags & UIFlag_VerticalCenter)
	{
		object.position.y += (parentRect.scale.y / 2) + parentRect.position.y;
	}

	if (object.type == UIType_Text)
	{
		object.text = uiComponent["text"].get<std::string>();
		object.fontSize = uiComponent["fontSize"].get<int>();
	}

	UIManager::AddObject(object);

	sol::optional<sol::table> childrenExist = uiComponent["children"];

	if (childrenExist == sol::nullopt)
		return;

	sol::table children = uiComponent["children"];

	unsigned int index = 0;

	UIRect currentRect = { object.position, object.scale, 0.0f };

	while (true)
	{
		sol::optional<sol::table> uiComponentExist = children[index];

		if (uiComponentExist == sol::nullopt)
			return;

		sol::table uiComponent = children[index];
		CreateUIComponent(uiComponent, currentRect);
		index++;
	}
}