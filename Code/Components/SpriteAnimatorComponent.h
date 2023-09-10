#pragma once

#include <vector>
#include <imgui/imgui.h>
#include "imgui/imgui_stdlib.h"
#include "Animations/Sprite.h"
#include "Animations/SpriteAnimation.h"
#include "Animations/AnimatorController.h"
#include "Utils/FileDialog.h"

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

static void RenderImGui(SpriteAnimatorComponent& anim)
{
	std::string directory;
	ImGui::InputText("Controller Name", &anim.controllerName); ImGui::SameLine();
	if (ImGui::Button("Browse"))
	{
		directory = FileDialog::OpenFile("");
		anim.controllerName = directory;
	}
}