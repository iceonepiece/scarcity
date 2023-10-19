#pragma once

#include <glm/glm.hpp>
#include <string>
#include "UI/UIButton.h"

struct ButtonComponent
{
	static std::string Name() { return "Button"; }

	std::string text;
	glm::vec4 color{ 1.0f };

	//UIButton* instance = nullptr;
	UIButton instance;
};

static void AdjustCanvas(ButtonComponent& button, CanvasComponent* canvas)
{
	//button.instance.SetCanvas(canvas);
}

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