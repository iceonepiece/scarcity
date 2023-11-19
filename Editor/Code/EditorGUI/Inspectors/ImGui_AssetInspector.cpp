#include "ImGui_AssetInspector.h"

#include <imgui/imgui.h>
#include "ImGui_AssetInspector.h"
#include "Graphics/Texture.h"
#include <iostream>
#include "File/MetaSerializer.h"
#include "Audio/AudioSource.h"
#include "../ImGuiComponents/ImGuiComponentRenderer.h"

void ImGui_AssetInspector::Render(Asset* asset)
{
    if (TextureAsset* textureAsset = dynamic_cast<TextureAsset*>(asset))
        RenderTextureAsset(*textureAsset);

    else if (SpriteAsset* spriteAsset = dynamic_cast<SpriteAsset*>(asset))
        RenderSpriteAsset(*spriteAsset);

    else if (PrefabAsset* prefabAsset = dynamic_cast<PrefabAsset*>(asset))
        RenderPrefabAsset(*prefabAsset);

    else if (AudioAsset* audioAsset = dynamic_cast<AudioAsset*>(asset))
        RenderAudioAsset(*audioAsset);

    else if (AnimatorControllerAsset* animatorAsset = dynamic_cast<AnimatorControllerAsset*>(asset))
        RenderAnimatorControllerAsset(*animatorAsset);
}

void ImGui_AssetInspector::RenderAnimatorControllerAsset(AnimatorControllerAsset& animatorAsset)
{
    std::string name = animatorAsset.GetPath().stem().string() + " (Animator Controller)";

    ImGui::Text(name.c_str());

    if (AnimatorController* animController = animatorAsset.GetController())
    {
        auto& parameters = animController->GetParameters();

        for (auto& p : parameters)
        {
            std::string typeStr;

            /*
            if (auto ptr = std::get_if<float>(&p.second))
                typeStr = "Float";
            else if (auto ptr = std::get_if<int>(&p.second))
                typeStr = "Int";
            else if (auto ptr = std::get_if<bool>(&p.second))
                typeStr = "Bool";
            else if (auto ptr = std::get_if<Trigger>(&p.second))
                typeStr = "Trigger";

            ImGui::Text((p.first + " " + typeStr).c_str());
            */
        }
    }
}

void ImGui_AssetInspector::RenderPrefabAsset(PrefabAsset& prefabAsset)
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

void ImGui_AssetInspector::RenderTextureAsset(TextureAsset& textureAsset)
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

    AssetManager& assetManager = Application::Get().GetAssetManager();

    if (assetManager.HasTexture(pathString))
    {
        texture = &assetManager.GetTexture(pathString);
    }
    else
    {
        std::cout << "Load when clicked: " << pathString << std::endl;
        texture = assetManager.LoadTexture(pathString, pathString.c_str());
    }

    if (texture != nullptr)
    {
        ImGui::Text("pointer = %p", texture->GetRendererID());
        ImGui::Text("size = %d x %d", texture->GetWidth(), texture->GetHeight());
        ImGui::Image((ImTextureID)texture->GetRendererID(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()), { 0, 1 }, { 1, 0 });
    }
}

void ImGui_AssetInspector::RenderAudioAsset(AudioAsset& audioAsset)
{
    std::string name = audioAsset.GetName() + " (Audio)";

    ImGui::Text(name.c_str());

    if (AudioClip* audioClip = audioAsset.GetAudioClip())
    {
        std::string durationText = "Seconds: " + std::to_string(audioClip->GetDuration());
        ImGui::Text(durationText.c_str());
        if (ImGui::Button("Play"))
        {
            Application::Get().GetAudio().PlaySound(audioClip);
        }

    }
}

void ImGui_AssetInspector::RenderSpriteAsset(SpriteAsset& spriteAsset)
{
    std::string name = spriteAsset.GetSprite().GetName() + " (Sprite)";
    ImGui::Text(name.c_str());

    std::string pathString = spriteAsset.GetTextureAsset().GetPath().string();

    Texture* texture = nullptr;
    AssetManager& assetManager = Application::Get().GetAssetManager();

    if (assetManager.HasTexture(pathString))
    {
        texture = &assetManager.GetTexture(pathString);
    }
    else
    {
        std::cout << "Load when clicked: " << pathString << std::endl;
        texture = assetManager.LoadTexture(pathString, pathString.c_str());
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

        ImVec2 renderSize{ (float)renderWidth, renderWidth * (sprite.GetRatio().y / sprite.GetRatio().x) };
        ImGui::Image((ImTextureID)texture->GetRendererID(), renderSize, { sprite.GetLeft(), sprite.GetTop() }, { sprite.GetRight(), sprite.GetBottom() });
    }
}