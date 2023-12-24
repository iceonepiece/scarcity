#pragma once

#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <box2d/b2_body.h>
#include "Components/Collider2DComponent.h"

struct CircleCollider2DComponent : public Collider2DComponent
{
	static std::string Name() { return "CircleCollider2D"; }
	virtual ColliderType GetType() const { return ColliderType::Circle; }

	float radius = 0.5;
};

static void DoSerialize(const CircleCollider2DComponent& circle, json& entityJson)
{
	json offset = json::object();
	offset["x"] = circle.offset.x;
	offset["y"] = circle.offset.y;
	entityJson[circle.Name()]["offset"] = offset;
	entityJson[circle.Name()]["radius"] = circle.radius;
	entityJson[circle.Name()]["isTrigger"] = circle.isTrigger;
}

static void DoDeserialize(CircleCollider2DComponent& circle, json& circleJson)
{
	auto& offsetJson = circleJson["offset"];
	circle.offset = { offsetJson["x"].get<float>(), offsetJson["y"].get<float>() };
	circle.radius = circleJson["radius"].get<float>();
	circle.isTrigger = circleJson["isTrigger"].get<bool>();
}