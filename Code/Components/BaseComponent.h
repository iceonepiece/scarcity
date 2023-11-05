#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct BaseComponent
{
	static std::string Name() { return "Base"; }

	std::string name = "";
	std::string tag = "Untagged";
};

static void DoSerialize(const BaseComponent& base, json& entityJson)
{
	entityJson["Base"]["name"] = base.name;
	entityJson["Base"]["tag"] = base.tag;
}

static void DoDeserialize(BaseComponent& base, json& baseJson)
{
	base.name = baseJson["name"].get<std::string>();

	if (baseJson["tag"].is_number())
		base.tag = baseJson["tag"] = "Untagged";
	else
		base.tag = baseJson["tag"].get<std::string>();
}