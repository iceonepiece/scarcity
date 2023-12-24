#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include "Collider2DComponent.h"

using json = nlohmann::json;

struct Collider2DData
{
	ColliderType type = ColliderType::Box;
	uint16_t layer = 0;
	bool isTrigger = false;
	glm::vec2 offset = { 0.0f, 0.0f };
	glm::vec2 size = { 0.0f, 0.0f };
	float density = 1.0f;
	float friction = 0.0f;
};

struct Collider2DGroupComponent
{
	static std::string Name() { return "Collider2DGroup"; }

	std::vector<Collider2DData> colliders;
};

static void DoSerialize(const Collider2DGroupComponent& group, json& entityJson)
{
	json collidersJson = json::array();

	for (auto& collider : group.colliders)
	{
		json colliderJson = json::object();

		colliderJson["type"] = collider.type;
		colliderJson["layer"] = collider.layer;
		colliderJson["isTrigger"] = collider.isTrigger;

		json offset = json::object();
		offset["x"] = collider.offset.x;
		offset["y"] = collider.offset.y;

		colliderJson["offset"] = offset;

		json size = json::object();
		size["x"] = collider.size.x;
		size["y"] = collider.size.y;

		colliderJson["size"] = size;

		collidersJson.push_back(colliderJson);
	}

	entityJson[Collider2DGroupComponent::Name()]["colliders"] = collidersJson;
}

static void DoDeserialize(Collider2DGroupComponent& group, json& groupJson)
{
	json collidersJson = groupJson["colliders"];

	for (auto& colliderJson : collidersJson)
	{
		auto& offsetJson = colliderJson["offset"];
		glm::vec2 offset{ offsetJson["x"].get<float>(), offsetJson["y"].get<float>() };

		auto& sizeJson = colliderJson["size"];
		glm::vec2 size{ sizeJson["x"].get<float>(), sizeJson["y"].get<float>() };

		Collider2DData collider;

		collider.type = colliderJson["type"].get<ColliderType>();
		collider.layer = colliderJson["layer"].get<uint16_t>();
		collider.isTrigger = colliderJson["isTrigger"].get<bool>();
		collider.offset = offset;
		collider.size = size;

		group.colliders.push_back(collider);
	}
}