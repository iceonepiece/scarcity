#include "UIManager.h"
#include "Math/Math.h"
#include <iostream>

UIRect UIManager::GetOffsetRect()
{
	if (!m_rectStack.empty())
		return m_rectStack.back();

	return UIRect{
		 { m_screenSize.x / 2, m_screenSize.y / 2 },
		m_screenSize,
	};
}

glm::vec2 UIManager::GetAbsolutePosition(float width, float height, UIFlag flags)
{
	UIRect offsetRect = GetOffsetRect();

	glm::vec2 topLeft = offsetRect.position;
	topLeft.x -= offsetRect.scale.x / 2;
	topLeft.y -= offsetRect.scale.y / 2;

	glm::vec3 absolutePosition{ topLeft.x, topLeft.y, 0.0f };

	if (flags == UIFlag::UIFlag_None)
	{
		absolutePosition.x += width / 2;
		absolutePosition.y += height / 2;
	}
	else
	{
		if (flags & UIFlag::UIFlag_VerticalCenter)
		{
			absolutePosition.y += offsetRect.scale.y / 2;
		}

		if (flags & UIFlag::UIFlag_HorizontalCenter)
		{
			absolutePosition.x += offsetRect.scale.x / 2;
		}

		if (flags & UIFlag::UIFlag_Right)
		{
			absolutePosition.x = offsetRect.scale.x;
			absolutePosition.x -= width / 2;
		}

		if (flags & UIFlag::UIFlag_Bottom)
		{
			absolutePosition.y = offsetRect.scale.y;
			absolutePosition.y -= height / 2;
		}

		if (flags & UIFlag::UIFlag_Left)
		{
			absolutePosition.x = topLeft.x;
			absolutePosition.x += width / 2;
		}

		if (flags & UIFlag::UIFlag_Top)
		{
			absolutePosition.y = topLeft.y;
			absolutePosition.y += height / 2;
		}
	}

	return absolutePosition;
}

void UIManager::Clear()
{
	for (auto& object : m_objects)
	{
		object.reset();
	}

	m_objects.clear();
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

	UIObject* object = nullptr;

	UIType uiType = (UIType)uiComponent["class"].get<int>();

	if (uiType == UIType_Button)
	{
		object = new UIButtonObject();
		object->onHoverFunction = uiComponent["onHover"];
	}
	else
	{
		object = new UIObject();
	}

	object->position = { x + parentRect.position.x, y + parentRect.position.y, 0 };
	object->scale = { w,  h, 0 };
	object->color = color;
	object->flags = (UIFlag)uiComponent["flags"].get<int>();

	if (uiType == UIType_Text)
	{
		glm::vec4 fontColor = {
			uiComponent["fontColor"][1],
			uiComponent["fontColor"][2],
			uiComponent["fontColor"][3],
			uiComponent["fontColor"][4]
		};

		object->fontColor = fontColor;
	}

	if (object->flags & UIFlag_HorizontalCenter)
	{
		object->position.x += (parentRect.scale.x / 2) + parentRect.position.x;
	}

	if (object->flags & UIFlag_VerticalCenter)
	{
		object->position.y += (parentRect.scale.y / 2) + parentRect.position.y;
	}

	if (object->Type() == UIType_Text)
	{
		object->text = uiComponent["text"].get<std::string>();
		object->fontSize = uiComponent["fontSize"].get<int>();
	}

	UIManager::AddObject(object);

	sol::optional<sol::table> childrenExist = uiComponent["children"];

	if (childrenExist == sol::nullopt)
		return;

	sol::table children = uiComponent["children"];

	unsigned int index = 1;

	UIRect currentRect = { object->position, object->scale, 0.0f };

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