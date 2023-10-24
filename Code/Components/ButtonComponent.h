#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Components/UIComponent.h"
#include "Core/UniqueID.h"
#include "Core/Entity.h"
#include "UI/EventHandler.h"

struct ButtonComponent : public UIComponent
{
	static std::string Name() { return "Button"; }

	std::string text;

	EventHandler<UIComponent&> mousePressedHandler;
	EventHandler<UIComponent&> mouseReleasedHandler;

	UniqueID targetID;
	Entity targetEntity;

	std::string functionName;
	std::vector<std::string> functionNames;

	ButtonComponent()
	{
		mouseEnterHandler += [&](void*, UIComponent& component)
		{
			component.color *= 1.25f;
		};

		mouseLeaveHandler += [&](void*, UIComponent& component)
		{
			component.color /= 1.25f;
		};
	}
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

	entityJson[ButtonComponent::Name()]["targetID"] = (uint64_t)button.targetID;
	entityJson[ButtonComponent::Name()]["functionName"] = button.functionName;
}

static void DoDeserialize(ButtonComponent& button, json& buttonJson)
{
	button.text = buttonJson["text"].get<std::string>();

	auto& colorJson = buttonJson["color"];
	glm::vec4 color{ colorJson[0].get<float>(), colorJson[1].get<float>(), colorJson[2].get<float>(), colorJson[3].get<float>() };
	button.color = color;

	if (buttonJson["targetID"].is_number_unsigned())
		button.targetID = buttonJson["targetID"].get<uint64_t>();

	if (buttonJson["functionName"].is_string())
		button.functionName = buttonJson["functionName"].get<std::string>();
}