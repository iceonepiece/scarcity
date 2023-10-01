#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "Core/UUID.h"

using json = nlohmann::json;

struct IDComponent
{
	static std::string Name() { return "ID"; }

	UUID ID;
};

static void DoSerialize(const IDComponent& id, json& entityJson)
{

}

static void DoDeserialize(IDComponent& id, json& baseJson)
{

}