#pragma once

#include "Lua/LuaScript.h"
#include "Core/Application.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct LuaScriptComponent
{
	static std::string Name() { return "LuaScript"; }

	LuaScript* script = nullptr;
};

static void DoSerialize(const LuaScriptComponent& luaScript, json& entityJson)
{
	std::string scriptPath = "";

	if (luaScript.script != nullptr)
		scriptPath = luaScript.script->GetPath().string();

	entityJson[LuaScriptComponent::Name()]["scriptPath"] = scriptPath;
}

static void DoDeserialize(LuaScriptComponent& luaScript, json& luaScriptJson)
{
	if (luaScriptJson["scriptPath"].is_string())
	{
		if (Asset* asset = Application::Get().GetAssetManager().GetAsset(luaScriptJson["scriptPath"]))
		{
			luaScript.script = dynamic_cast<LuaScript*>(asset);
		}
	}
}