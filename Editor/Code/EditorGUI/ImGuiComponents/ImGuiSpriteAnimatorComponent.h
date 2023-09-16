#pragma once

#include "../Windows/ImGuiSelectAnimatorControllerWindow.h"
#include "../../EditorLayer.h"
#include "Utils/FileDialog.h"

static void RenderImGui(SpriteAnimatorComponent& spriteAnimator)
{
	if (ImGui::Button("Browse"))
		ImGui::OpenPopup("Select AnimatorController");

	ImGui::SameLine();
	ImGui::PushItemWidth(180);
	ImGui::Text(("Controller: " + spriteAnimator.controllerName).c_str());
	ImGui::PopItemWidth();

	/*
	if (ImGui::BeginPopupModal("Select AnimatorController"))
	{
		ImGuiSelectAnimatorControllerWindow* window = dynamic_cast<ImGuiSelectAnimatorControllerWindow*>(EditorLayer::GetImGuiWindow(ImGuiWindowType::SelectAnimatorController));
		window->Render();

		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			spriteAnimator.controllerName = window->GetSelectedControllerName();
			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	*/
}