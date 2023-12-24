#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <imgui/imgui.h>
#include "Core/Application.h"
#include "Graphics/Image.h"

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
	int order = 0;

	Image* image = nullptr;
	size_t spriteIndex;

	Sprite* sprite = nullptr;
};

static void DoSerialize(const SpriteRendererComponent& sprite, json& entityJson)
{
	entityJson[SpriteRendererComponent::Name()]["shape"] = sprite.shape;

	json colorJson = json::array();
	colorJson.push_back(sprite.color.x);
	colorJson.push_back(sprite.color.y);
	colorJson.push_back(sprite.color.z);
	colorJson.push_back(sprite.color.w);
	entityJson[SpriteRendererComponent::Name()]["color"] = colorJson;
	entityJson[SpriteRendererComponent::Name()]["order"] = sprite.order;

	json spriteJson = json::object();
	spriteJson["imageID"] = 0;

	if (sprite.image != nullptr)
	{
		spriteJson["imageID"] = (uint64_t)sprite.image->GetID();
		spriteJson["index"] = (size_t)sprite.spriteIndex;
	}

	entityJson[SpriteRendererComponent::Name()]["sprite"] = spriteJson;
}

static void DoDeserialize(SpriteRendererComponent& sprite, json& spriteRendererJson)
{
	auto& colorJson = spriteRendererJson["color"];
	glm::vec4 color { colorJson[0].get<float>(), colorJson[1].get<float>(), colorJson[2].get<float>(), colorJson[3].get<float>() };

	sprite.shape = spriteRendererJson["shape"].get<SpriteShape>();
	sprite.color = color;

	if (spriteRendererJson["order"].is_null())
		sprite.order = 0;
	else
		sprite.order = spriteRendererJson["order"].get<int>();

	UniqueID imageID = spriteRendererJson["sprite"]["imageID"].get<uint64_t>();
	if (Image* image = dynamic_cast<Image*>(Application::Get().GetAssetManager().GetAssetByID(imageID)))
	{
		sprite.image = image;
		sprite.spriteIndex = spriteRendererJson["sprite"]["index"].get<size_t>();
		sprite.spriteName = image->GetSprites()[sprite.spriteIndex].GetName();
		sprite.sprite = &image->GetSprites()[sprite.spriteIndex];
	}
}