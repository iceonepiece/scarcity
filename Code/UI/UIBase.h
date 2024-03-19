#pragma once

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
	UIType_Image
};

struct UIRect
{
	glm::vec2 position = { 0.0f, 0.0f };
	glm::vec2 scale = { 0.0f, 0.0f };
	float rotation = 0.0f;
};
