#pragma once

#include "Components/Components.h"
#include "../Windows/ImGuiSelectSpriteWindow.h"
#include "../../EditorLayer.h"

static void RenderImGui(SpriteRendererComponent& spriteRenderer)
{
	const char* shapeTypeStrings[] = { "None", "Square", "Circle" };
	const char* currentShapeTypeString = shapeTypeStrings[(int)spriteRenderer.shape];
	if (ImGui::BeginCombo("Shape Type", currentShapeTypeString))
	{
		for (int i = 0; i < 3; i++)
		{
			bool isSelected = currentShapeTypeString == shapeTypeStrings[i];
			if (ImGui::Selectable(shapeTypeStrings[i], isSelected))
			{
				currentShapeTypeString = shapeTypeStrings[i];
				spriteRenderer.shape = (SpriteShape)i;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	if (ImGui::Button("Browse"))
		ImGui::OpenPopup("Select Sprite");

	ImGui::SameLine();
	ImGui::PushItemWidth(180);
	ImGui::Text(("Sprite: " + spriteRenderer.spriteName).c_str());
	ImGui::PopItemWidth();

	if (ImGui::BeginPopupModal("Select Sprite"))
	{
		ImGuiSelectSpriteWindow* window = dynamic_cast<ImGuiSelectSpriteWindow*>(EditorLayer::GetImGuiWindow(ImGuiWindowType::SelectSprite));
		window->Render();

		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			Sprite* sprite = window->GetSelectedSprite();

			if (sprite)
			{
				spriteRenderer.sprite = sprite;
				spriteRenderer.spriteName = sprite->GetName();
			}

			//EditorLayer::GetAsset(selectedPath);

			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	/*
	ImGui::InputText("Sprite Name", &spriteRenderer.spriteName); ImGui::SameLine();
	if (ImGui::Button("Load Sprite"))
	{
		spriteRenderer.sprite = ResourceAPI::GetResourceManager()->GetSprite(spriteRenderer.spriteName);
	}
	*/
}