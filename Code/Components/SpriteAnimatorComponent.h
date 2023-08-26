#pragma once

#include <vector>
#include <imgui/imgui.h>
#include "imgui/imgui_stdlib.h"
#include "../Animations/Sprite.h"
#include "../Animations/SpriteAnimation.h"
#include "Utils/FileDialog.h"

struct SpriteAnimatorComponent
{
	static std::string Name() { return "SpriteAnimator"; }

	FiniteStateMachine* fsm = nullptr;
	std::string fileName;

	SpriteAnimatorComponent() = default;

	SpriteAnimatorComponent(FiniteStateMachine *fsm)
		: fsm(fsm)
	{}
};

static void DoSerialize(const SpriteAnimatorComponent& animator, json& entityJson)
{
	entityJson["SpriteAnimator"]["fileName"] = animator.fileName;
}

static void DoDeserialize(SpriteAnimatorComponent& animator, json& animatorJson)
{
	animator.fileName = animatorJson["fileName"];
}

static void RenderImGui(SpriteAnimatorComponent& anim)
{
	std::string directory;
	ImGui::InputText("File Name", &anim.fileName); ImGui::SameLine();
	if (ImGui::Button("Browse"))
	{
		directory = FileDialog::OpenFile("");
		anim.fileName = directory;
	}
}