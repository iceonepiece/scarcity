#pragma once

#include "Lua/LuaScript.h"
#include "Core/Application.h"
#include "Project/Project.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct LuaScriptComponent
{
	static std::string Name() { return "LuaScript"; }

	LuaScript* script = nullptr;
	LuaEngine* engine = nullptr;
};

static void DoSerialize(const LuaScriptComponent& luaScript, json& entityJson)
{
	std::filesystem::path scriptPath = "";

	if (luaScript.script != nullptr)
	{
		scriptPath = std::filesystem::relative(luaScript.script->GetPath(), Project::GetActive()->GetDirectory());
	}

	entityJson[LuaScriptComponent::Name()]["scriptPath"] = scriptPath.string();
}

static void DoDeserialize(LuaScriptComponent& luaScript, json& luaScriptJson)
{
	if (luaScriptJson["scriptPath"].is_string())
	{
		if (Asset* asset = Project::GetActive()->GetAsset(luaScriptJson["scriptPath"]))
		{
			luaScript.script = dynamic_cast<LuaScript*>(asset);
		}
	}
}