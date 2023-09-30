#pragma once

#include <string>

struct CanvasComponent
{
	static std::string Name() { return "Canvas"; }

	float x = 0.0f;
	float y = 0.0f;
	float width = 1.0f;
	float height = 1.0f;
};

static void DoSerialize(const CanvasComponent& canvas, json& entityJson)
{

}

static void DoDeserialize(CanvasComponent& canvas, json& cameraJson)
{

}

struct TextComponent
{

};

struct ImageComponent
{

};

struct ButtonComponent
{
	static std::string Name() { return "Button"; }

	std::string text;
};

static void DoSerialize(const ButtonComponent& button, json& entityJson)
{

}

static void DoDeserialize(ButtonComponent& button, json& cameraJson)
{

}
