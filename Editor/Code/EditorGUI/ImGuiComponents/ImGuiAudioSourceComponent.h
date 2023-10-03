#pragma once

#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include "Components/AudioSourceComponent.h"

static void RenderImGui(AudioSourceComponent& audioSource)
{
	ImGui::InputText("Audio Clip", &audioSource.audioClipPath);
	ImGui::DragFloat("Volume", &audioSource.volume);
}