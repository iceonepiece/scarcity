#pragma once

#include <glm/glm.hpp>

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
};

struct UIRect
{
	glm::vec2 position;
	glm::vec2 scale;
	float rotation;
};

class UIObject
{
public:
	UIType type = UIType_Box;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec4 color { 1.0f, 1.0f, 1.0f, 1.0f };

	UIFlag flags = UIFlag_None;

	std::string text;
	int fontSize = 18;

	glm::vec3 absolutePosition;
	glm::vec3 absoluteScale;
	glm::vec3 absoluteRotation;
};

