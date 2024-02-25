#pragma once

#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include "Input/Input.h"
#include "Math/Math.h"

enum UIFlag
{
	UIFlag_None = 0,
	UIFlag_Left = 1,
	UIFlag_Right = 1 << 1,
	UIFlag_Top = 1 << 2,
	UIFlag_Bottom = 1 << 3,
	UIFlag_VerticalCenter = 1 << 4,
	UIFlag_HorizontalCenter = 1 << 5,
};

enum UIType
{
	UIType_Box,
	UIType_Text,
	UIType_Button,
};

struct UIRect
{
	glm::vec2 position = { 0.0f, 0.0f };
	glm::vec2 scale = { 0.0f, 0.0f };
	float rotation = 0.0f;
};

class UIObject
{
public:
	virtual ~UIObject() = default;

	bool active = false;

	UIType type = UIType_Box;
	glm::vec3 position{ 0.0f };
	glm::vec3 scale{ 1.0f };
	glm::vec3 rotation{ 0.0f, 0.0f, 1.0f };
	glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 fontColor{ 0.1f, 0.1f, 0.1f, 1.0f };

	glm::vec4 backgroundColor { 0, 0, 0, 1 };
	glm::vec4 onHoverColor{ 0.8f, 0.8f, 0.1f, 1.0f };

	UIFlag flags = UIFlag_None;

	std::string text;
	int fontSize = 18;

	glm::vec3 absolutePosition;
	glm::vec3 absoluteScale;
	glm::vec3 absoluteRotation;

	sol::function onHoverFunction;
	sol::function onClickFunction;

	virtual void HandleInput(Input& input) { color = backgroundColor; }
};

class UIButtonObject : public UIObject
{
public:
	~UIButtonObject() = default;

	virtual void HandleInput(Input& input) override
	{
		color = backgroundColor;
		if (Math::Contains(position, scale, input.GetCursorPosition()))
		{
			color.x *= 1.5f;
			color.y *= 1.5f;
			color.z *= 1.5f;

			if (onHoverFunction.valid())
				onHoverFunction();

			if (input.GetMouseButtonDown(Mouse::ButtonLeft))
			{
				if (onClickFunction.valid())
					onClickFunction();
			}
		}
	}
};
