#pragma once

#include <string>
#include "UI/UIButton.h"

struct CanvasComponent
{
	static std::string Name() { return "Canvas"; }

	glm::vec2 position { 0.0f, 0.0f };
	glm::vec2 size { 1.0f, 1.0f };
};

static void DoSerialize(const CanvasComponent& canvas, json& entityJson)
{
	json positionJson = json::array();
	positionJson.push_back(canvas.position.x);
	positionJson.push_back(canvas.position.y);
	entityJson[CanvasComponent::Name()]["position"] = positionJson;

	json sizeJson = json::array();
	sizeJson.push_back(canvas.size.x);
	sizeJson.push_back(canvas.size.y);
	entityJson[CanvasComponent::Name()]["size"] = sizeJson;
}

static void DoDeserialize(CanvasComponent& canvas, json& canvasJson)
{
	auto& positionJson = canvasJson["position"];
	glm::vec2 position { positionJson[0].get<float>(), positionJson[1].get<float>() };

	auto& sizeJson = canvasJson["size"];
	glm::vec2 size { sizeJson[0].get<float>(), sizeJson[1].get<float>() };

	canvas.position = position;
	canvas.size = size;
}

struct TextComponent
{
	static std::string Name() { return "Text"; }

	std::string text = "";
	float size = 16;
	glm::vec4 color { 1.0f };
};

static void DoSerialize(const TextComponent& text, json& entityJson)
{
	entityJson[TextComponent::Name()]["text"] = text.text;
	entityJson[TextComponent::Name()]["size"] = text.size;

	json colorJson = json::array();
	colorJson.push_back(text.color.r);
	colorJson.push_back(text.color.g);
	colorJson.push_back(text.color.b);
	colorJson.push_back(text.color.a);
	entityJson[TextComponent::Name()]["color"] = colorJson;
}

static void DoDeserialize(TextComponent& text, json& textJson)
{
	text.text = textJson["text"].get<std::string>();
	text.size = textJson["size"].get<float>();

	auto& colorJson = textJson["color"];
	glm::vec4 color{ colorJson[0].get<float>(), colorJson[1].get<float>(), colorJson[2].get<float>(), colorJson[3].get<float>() };
	text.color = color;
}


struct ImageComponent
{

};

struct ButtonComponent
{
	static std::string Name() { return "Button"; }

	std::string text;
	glm::vec4 color{ 1.0f };

	UIButton instance;
};

static void DoSerialize(const ButtonComponent& button, json& entityJson)
{
	entityJson[ButtonComponent::Name()]["text"] = button.text;

	json colorJson = json::array();
	colorJson.push_back(button.color.r);
	colorJson.push_back(button.color.g);
	colorJson.push_back(button.color.b);
	colorJson.push_back(button.color.a);
	entityJson[ButtonComponent::Name()]["color"] = colorJson;
}

static void DoDeserialize(ButtonComponent& button, json& buttonJson)
{
	button.text = buttonJson["text"].get<std::string>();

	auto& colorJson = buttonJson["color"];
	glm::vec4 color{ colorJson[0].get<float>(), colorJson[1].get<float>(), colorJson[2].get<float>(), colorJson[3].get<float>() };
	button.color = color;
}
