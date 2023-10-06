#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "Core/UniqueID.h"
#include <cstdint>

using json = nlohmann::json;

struct IDComponent
{
	static std::string Name() { return "ID"; }

	UniqueID ID;
};

static void DoSerialize(const IDComponent& id, json& entityJson)
{

}

static void DoDeserialize(IDComponent& id, json& idJson)
{

}