#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
#include "Components/Collider2DComponent.h"

using json = nlohmann::json;

struct BoxCollider2DComponent : public Collider2DComponent
{
	static std::string Name() { return "BoxCollider2D"; }

	glm::vec2 offset = { 0.0f, 0.0f };
	glm::vec2 size = { 1.0f, 1.0f };
};

static void DoSerialize(const BoxCollider2DComponent& box, json& entityJson)
{
	json offset = json::object();
	offset["x"] = box.offset.x;
	offset["y"] = box.offset.y;

	entityJson["BoxCollider2D"]["offset"] = offset;

	json size = json::object();
	size["x"] = box.size.x;
	size["y"] = box.size.y;

	entityJson["BoxCollider2D"]["size"] = size;
	entityJson["BoxCollider2D"]["isTrigger"] = box.isTrigger;
}

static void DoDeserialize(BoxCollider2DComponent& box, json& boxJson)
{
	auto& offsetJson = boxJson["offset"];
	glm::vec2 offset { offsetJson["x"].get<float>(), offsetJson["y"].get<float>() };

	auto& sizeJson = boxJson["size"];
	glm::vec2 size { sizeJson["x"].get<float>(), sizeJson["y"].get<float>() };

	box.offset = offset;
	box.size = size;
	box.isTrigger = boxJson["isTrigger"].get<bool>();
}