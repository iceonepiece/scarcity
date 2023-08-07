#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct BaseComponent
{
	static std::string Name() { return "Base"; }

	std::string name;
};

static void DoSerialize(const BaseComponent& base, json& entityJson)
{
	entityJson["Base"]["name"] = base.name;
}

static void DoDeserialize(BaseComponent& base, json& baseJson)
{
	base.name = baseJson["name"].get<std::string>();
}