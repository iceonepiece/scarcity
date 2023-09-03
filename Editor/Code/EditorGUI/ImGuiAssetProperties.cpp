#include <imgui/imgui.h>
#include "ImGuiAssetProperties.h"
#include "Graphics/Texture.h"
#include "Core/ResourceAPI.h"
#include <iostream>
#include "File/MetaSerializer.h"

void ImGuiAssetProperties::Render(Asset* asset)
{
    if (TextureAsset* sprite = dynamic_cast<TextureAsset*>(asset))
    {
        std::string name = asset->GetName() + " (Texture 2D)";
        ImGui::Text(name.c_str());

        const char* spriteModeStrings[] = { "Single", "Multiple" };
        const char* currentSpriteModeString = spriteModeStrings[(int)sprite->GetSpriteMode()];
        if (ImGui::BeginCombo("Sprite Mode", currentSpriteModeString))
        {
            for (int i = 0; i < 2; i++)
            {
                bool isSelected = currentSpriteModeString == spriteModeStrings[i];
                if (ImGui::Selectable(spriteModeStrings[i], isSelected))
                {
                    currentSpriteModeString = spriteModeStrings[i];
                    sprite->SetSpriteMode((SpriteMode)i);
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        if (sprite->GetSpriteMode() == SpriteMode::Multiple)
        {
            ImGui::InputInt("Rows", &sprite->GetRows());
            ImGui::InputInt("Cols", &sprite->GetCols());
        }

        if (ImGui::Button("Revert"))
        {
            //RevertSpriteData();
        }

        ImGui::SameLine();

        if (ImGui::Button("Apply"))
        {
            MetaSerializer::SerializeImage(*sprite, sprite->GetPath());
        }

        std::string pathString = asset->GetPath().string();

        Texture* texture = nullptr;

        if (ResourceAPI::HasTexture(pathString))
        {
            texture = &ResourceAPI::GetTexture(pathString);
        }
        else
        {
            std::cout << "Load when clicked: " << pathString << std::endl;
            texture = ResourceAPI::LoadTexture(pathString, pathString.c_str());
        }

        if (texture != nullptr)
        {
            ImGui::Text("pointer = %p", texture->GetRendererID());
            ImGui::Text("size = %d x %d", texture->GetWidth(), texture->GetHeight());
            ImGui::Image((ImTextureID)texture->GetRendererID(), ImVec2(texture->GetWidth(), texture->GetHeight()), { 0, 1 }, { 1, 0 });
        }
    }
}