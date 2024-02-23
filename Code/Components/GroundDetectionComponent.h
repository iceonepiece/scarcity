#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
#include "Physics/FixtureData.h"
#include "Components/GroundDetectionComponent.h"

using json = nlohmann::json;

struct GroundDetectionComponent
{
	static std::string Name() { return "GroundDetection"; }

	std::string groundTag = "Untagged";

	int numGrounds = 0;
	std::vector<b2Contact*> groundContacts;
	FixtureData* fixtureData = nullptr;
};

static void DoSerialize(const GroundDetectionComponent& groundDetection, json& entityJson)
{
	entityJson["GroundDetection"]["groundTag"] = groundDetection.groundTag;
}

static void DoDeserialize(GroundDetectionComponent& groundDetection, json& groundDetectionJson)
{
	if (groundDetectionJson["groundTag"].is_number())
		groundDetection.groundTag = "Untagged";
	else
		groundDetection.groundTag = groundDetectionJson["groundTag"].get<std::string>();
}