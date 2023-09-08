#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <imgui/imgui.h>
#include "Core/ResourceAPI.h"
#include "Core/ResourceManager.h"

using json = nlohmann::json;

enum SpriteShape
{
	Shape_None,
	Shape_Square,
	Shape_Circle
};

struct SpriteRendererComponent
{
	static std::string Name() { return "SpriteRenderer"; }

	SpriteShape shape = Shape_None;
	glm::vec4 color = glm::vec4 { 1.0f };
	std::string spriteName = "";
	Sprite* sprite = nullptr;
};

static void DoSerialize(const SpriteRendererComponent& sprite, json& entityJson)
{
	entityJson["SpriteRenderer"]["shape"] = sprite.shape;

	json colorJson = json::array();
	colorJson.push_back(sprite.color.x);
	colorJson.push_back(sprite.color.y);
	colorJson.push_back(sprite.color.z);
	colorJson.push_back(sprite.color.w);
	entityJson["SpriteRenderer"]["color"] = colorJson;
	entityJson["SpriteRenderer"]["spriteName"] = sprite.spriteName;
}

static void DoDeserialize(SpriteRendererComponent& sprite, json& spriteRendererJson)
{
	auto& colorJson = spriteRendererJson["color"];
	glm::vec4 color { colorJson[0].get<float>(), colorJson[1].get<float>(), colorJson[2].get<float>(), colorJson[3].get<float>() };

	sprite.shape = spriteRendererJson["shape"].get<SpriteShape>();
	sprite.color = color;

	sprite.spriteName = spriteRendererJson["spriteName"].get<std::string>();
	sprite.sprite = ResourceAPI::GetResourceManager()->GetSprite(sprite.spriteName);
}