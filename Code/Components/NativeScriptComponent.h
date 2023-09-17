#pragma once

#include <memory>
#include <string>
#include "Scene/ScriptableEntity.h"
#include <nlohmann/json.hpp>
#include <imgui/imgui.h>
#include "imgui/imgui_stdlib.h"

using json = nlohmann::json;

struct NativeScriptComponent
{
	static std::string Name() { return "NativeScript"; }

	std::string className;
	ScriptableEntity* instance = nullptr;

	/*
	ScriptableEntity* (*InstantiateScript)();
	void (*DestroyScript)(NativeScriptComponent*);

	template<typename T>
	void Bind()
	{
		InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
		DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
	}
	*/
};

static void DoSerialize(const NativeScriptComponent& nativeScript, json& entityJson)
{
	entityJson["NativeScript"]["className"] = nativeScript.className;
}

static void DoDeserialize(NativeScriptComponent& nativeScript, json& nativeScriptJson)
{
	nativeScript.className = nativeScriptJson["className"].get<std::string>();
}