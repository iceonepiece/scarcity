#pragma once

#include "../Windows/ImGuiSelectAnimatorControllerWindow.h"
#include "../../EditorLayer.h"
#include "Utils/FileDialog.h"
#include "../Windows/ImGui_WindowManager.h"

static void RenderImGui(SpriteAnimatorComponent& spriteAnimator)
{
	if (ImGui::Button("Browse"))
		ImGui::OpenPopup("Select AnimatorController");

	ImGui::SameLine();
	ImGui::PushItemWidth(180);

	std::string controllerName = spriteAnimator.prototypeController != nullptr ? spriteAnimator.prototypeController->GetName() : "";

	ImGui::Text(("Controller: " + controllerName).c_str());
	ImGui::PopItemWidth();

	if (ImGui::BeginPopupModal("Select AnimatorController"))
	{
		ImGuiSelectAnimatorControllerWindow* window = ImGuiSelectAnimatorControllerWindow::Get();
		window->Render();

		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			spriteAnimator.prototypeController = window->GetSelectedController();
			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}