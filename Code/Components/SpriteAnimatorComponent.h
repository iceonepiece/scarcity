#pragma once

#include <vector>
#include <nlohmann/json.hpp>
#include "Graphics/Sprite.h"
#include "Animations/SpriteAnimation.h"
#include "Animations/AnimatorController.h"

using json = nlohmann::json;

struct SpriteAnimatorComponent
{
	static std::string Name() { return "SpriteAnimator"; }

	static AnimatorController* CreateNullController()
	{
		return new AnimatorController();
	}

	AnimatorController* controller = nullptr;
	std::string controllerName;
};

static void DoSerialize(const SpriteAnimatorComponent& animator, json& entityJson)
{
	entityJson["SpriteAnimator"]["controllerName"] = animator.controllerName;
}

static void DoDeserialize(SpriteAnimatorComponent& animator, json& animatorJson)
{
	animator.controllerName = animatorJson["controllerName"];
}
