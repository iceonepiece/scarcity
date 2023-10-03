#include <imgui/imgui.h>
#include "ImGuiAssetProperties.h"
#include "Graphics/Texture.h"
#include "Core/ResourceAPI.h"
#include <iostream>
#include "File/MetaSerializer.h"
#include "ImGuiEntityProperties.h"
#include "Audio/AudioSource.h"
#include "ImGuiComponents/ImGuiComponentRenderer.h"


void ImGuiAssetProperties::Render(Asset* asset)
{
    if (TextureAsset* textureAsset = dynamic_cast<TextureAsset*>(asset))
        RenderTextureAsset(*textureAsset);

    else if (SpriteAsset* spriteAsset = dynamic_cast<SpriteAsset*>(asset))
        RenderSpriteAsset(*spriteAsset);

    else if (PrefabAsset* prefabAsset = dynamic_cast<PrefabAsset*>(asset))
        RenderPrefabAsset(*prefabAsset);

    else if (AudioAsset* audioAsset = dynamic_cast<AudioAsset*>(asset))
        RenderAudioAsset(*audioAsset);
}

void ImGuiAssetProperties::RenderPrefabAsset(PrefabAsset& prefabAsset)
{
    std::string name = prefabAsset.GetPath().stem().string() + " (Prefab)";
    ImGui::Text(name.c_str());

    Entity& entity = prefabAsset.GetEntity();
    auto& registry = entity.GetRegistry();

    ImGui::PushID((int)entity.GetEntity());

    std::apply([&](auto... componentTypes) {
        (ImGuiComponentRenderer::RenderComponent<decltype(componentTypes)>(registry, entity.GetEntity()), ...);
    }, ComponentList{});

    ImGui::PopID();
}

void ImGuiAssetProperties::RenderTextureAsset(TextureAsset& textureAsset)
{
    std::string name = textureAsset.GetName() + " (Texture 2D)";
    ImGui::Text(name.c_str());

    const char* spriteModeStrings[] = { "Single", "Multiple" };
    const char* currentSpriteModeString = spriteModeStrings[(int)textureAsset.GetSpriteMode()];
    if (ImGui::BeginCombo("Sprite Mode", currentSpriteModeString))
    {
        for (int i = 0; i < 2; i++)
        {
            bool isSelected = currentSpriteModeString == spriteModeStrings[i];
            if (ImGui::Selectable(spriteModeStrings[i], isSelected))
            {
                currentSpriteModeString = spriteModeStrings[i];
                textureAsset.SetSpriteMode((SpriteMode)i);
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    if (textureAsset.GetSpriteMode() == SpriteMode::Multiple)
    {
        ImGui::InputInt("Rows", &textureAsset.GetRows());
        ImGui::InputInt("Cols", &textureAsset.GetCols());
    }

    if (ImGui::Button("Revert"))
    {
        //RevertSpriteData();
    }

    ImGui::SameLine();

    if (ImGui::Button("Apply"))
    {
        if (MetaSerializer::SerializeImage(textureAsset, textureAsset.GetPath()))
            MetaSerializer::DeserializeImage(textureAsset, textureAsset.GetPath());
    }

    std::string pathString = textureAsset.GetPath().string();

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

void ImGuiAssetProperties::RenderAudioAsset(AudioAsset& audioAsset)
{
    std::string name = audioAsset.GetName() + " (Audio)";

    ImGui::Text(name.c_str());

    if (AudioSource* audioSource = audioAsset.GetAudioSource())
    {
        std::string durationText = "Seconds: " + std::to_string(audioSource->GetDuration());
        ImGui::Text(durationText.c_str());
        if (ImGui::Button("Play"))
        {
            audioSource->Play();
        }

    }
}

void ImGuiAssetProperties::RenderSpriteAsset(SpriteAsset& spriteAsset)
{
    std::string name = spriteAsset.GetSprite().GetName() + " (Sprite)";
    ImGui::Text(name.c_str());

    std::string pathString = spriteAsset.GetTextureAsset().GetPath().string();

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
        Sprite& sprite = spriteAsset.GetSprite();
        ImGui::Text("pointer = %p", texture->GetRendererID());
        ImGui::Text("size = %d x %d", (int)sprite.GetWidth(), (int)sprite.GetHeight());

        int renderWidth = 512;

        if (sprite.GetWidth() < 256)
            renderWidth = 256;
        else if (sprite.GetWidth() > 512)
            renderWidth = 512;

        ImVec2 renderSize { (float)renderWidth, renderWidth * (sprite.GetRatio().y / sprite.GetRatio().x) };
        ImGui::Image((ImTextureID)texture->GetRendererID(), renderSize, { sprite.GetLeft(), sprite.GetTop() }, {sprite.GetRight(), sprite.GetBottom() });
    }
}