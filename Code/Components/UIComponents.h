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
	static std::string Name() { return "Text"; }

	std::string text;
	float size;
};

static void DoSerialize(const TextComponent& text, json& entityJson)
{

}

static void DoDeserialize(TextComponent& text, json& textJson)
{

}


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
