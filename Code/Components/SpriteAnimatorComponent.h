#pragma once

#include <vector>
#include <nlohmann/json.hpp>
#include "Graphics/Sprite.h"
#include "Animations/AnimatorController.h"
#include "Core/Application.h"

using json = nlohmann::json;

struct SpriteAnimatorComponent
{
	static std::string Name() { return "SpriteAnimator"; }

	static AnimatorController* CreateNullController()
	{
		return new AnimatorController();
	}

	AnimatorController* prototypeController = nullptr;
	AnimatorController* controller = nullptr;
};

static void DoSerialize(const SpriteAnimatorComponent& animator, json& entityJson)
{
	entityJson[SpriteAnimatorComponent::Name()]["controllerID"] = animator.prototypeController != nullptr ? (uint64_t)animator.prototypeController->GetID() : 0;
}

static void DoDeserialize(SpriteAnimatorComponent& animator, json& animatorJson)
{
	if (animatorJson["controllerID"].is_number_unsigned())
	{
		uint64_t controllerID = animatorJson["controllerID"].get<uint64_t>();

		animator.prototypeController = dynamic_cast<AnimatorController*>(Project::GetActive()->GetAssetManager().GetAssetByID(controllerID));
	}
}
