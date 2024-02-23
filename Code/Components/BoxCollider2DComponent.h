#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
#include "Components/Collider2DComponent.h"

using json = nlohmann::json;

struct BoxCollider2DComponent : public Collider2DComponent
{
	static std::string Name() { return "BoxCollider2D"; }
	virtual ColliderType GetType() const { return ColliderType::Box; }

	glm::vec2 size = { 1.0f, 1.0f };
};

static void DoSerialize(const BoxCollider2DComponent& box, json& entityJson)
{
	entityJson[box.Name()]["layer"] = box.layer;
	entityJson[box.Name()]["isTrigger"] = box.isTrigger;

	json offset = json::object();
	offset["x"] = box.offset.x;
	offset["y"] = box.offset.y;

	entityJson[box.Name()]["offset"] = offset;

	json size = json::object();
	size["x"] = box.size.x;
	size["y"] = box.size.y;

	entityJson[box.Name()]["size"] = size;
}

static void DoDeserialize(BoxCollider2DComponent& box, json& boxJson)
{
	box.layer = boxJson["layer"].get<uint16_t>();
	box.isTrigger = boxJson["isTrigger"].get<bool>();

	auto& offsetJson = boxJson["offset"];
	glm::vec2 offset { offsetJson["x"].get<float>(), offsetJson["y"].get<float>() };

	auto& sizeJson = boxJson["size"];
	glm::vec2 size { sizeJson["x"].get<float>(), sizeJson["y"].get<float>() };

	box.offset = offset;
	box.size = size;
}