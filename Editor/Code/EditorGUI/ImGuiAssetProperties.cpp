#include <imgui/imgui.h>
#include "ImGuiAssetProperties.h"
#include "Graphics/Texture.h"
#include "Audio/Audio.h"
#include <iostream>
#include "File/MetaSerializer.h"
#include "Audio/AudioSource.h"
#include "ImGuiComponents/ImGuiComponentRenderer.h"


void ImGuiAssetProperties::Render(Asset* asset)
{
    if (Image* image = dynamic_cast<Image*>(asset))
        RenderImage(*image);

    else if (Sprite* sprite = dynamic_cast<Sprite*>(asset))
        RenderSprite(*sprite);

    else if (Prefab* prefab = dynamic_cast<Prefab*>(asset))
        RenderPrefab(*prefab);

    else if (AudioClip* audioClip = dynamic_cast<AudioClip*>(asset))
        RenderAudioClip(*audioClip);
}

void ImGuiAssetProperties::RenderPrefab(Prefab& prefab)
{
    std::string name = prefab.GetPath().stem().string() + " (Prefab)";
    ImGui::Text(name.c_str());

    Entity& entity = prefab.GetEntity();
    auto& registry = entity.GetRegistry();

    ImGui::PushID((int)entity.GetEntity());

    std::apply([&](auto... componentTypes) {
        (ImGuiComponentRenderer::RenderComponent<decltype(componentTypes)>(registry, entity.GetEntity()), ...);
    }, ComponentList{});

    ImGui::PopID();
}

void ImGuiAssetProperties::RenderImage(Image& image)
{
    std::string name = image.GetName() + " (Texture 2D)";
    ImGui::Text(name.c_str());

    const char* spriteModeStrings[] = { "Single", "Multiple" };
    const char* currentSpriteModeString = spriteModeStrings[(int)image.GetSpriteMode()];
    if (ImGui::BeginCombo("Sprite Mode", currentSpriteModeString))
    {
        for (int i = 0; i < 2; i++)
        {
            bool isSelected = currentSpriteModeString == spriteModeStrings[i];
            if (ImGui::Selectable(spriteModeStrings[i], isSelected))
            {
                currentSpriteModeString = spriteModeStrings[i];
                image.SetSpriteMode((SpriteMode)i);
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    if (image.GetSpriteMode() == SpriteMode::Multiple)
    {
        ImGui::InputInt("Rows", &image.GetRows());
        ImGui::InputInt("Cols", &image.GetCols());
    }

    if (ImGui::Button("Revert"))
    {
        //RevertSpriteData();
    }

    ImGui::SameLine();

    if (ImGui::Button("Apply"))
    {
        if (MetaSerializer::SerializeImage(image, image.GetPath()))
            MetaSerializer::DeserializeImage(image, image.GetPath());
    }

    std::string pathString = image.GetPath().string();

    Texture* texture = nullptr;

    AssetManager& assetManager = Project::GetActive()->GetAssetManager();

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

void ImGuiAssetProperties::RenderAudioClip(AudioClip& audioClip)
{
    std::string name = audioClip.GetName() + " (Audio)";

    ImGui::Text(name.c_str());

    std::string durationText = "Seconds: " + std::to_string(audioClip.GetDuration());
    ImGui::Text(durationText.c_str());
    if (ImGui::Button("Play"))
    {
        Audio::Get()->PlaySound(&audioClip);
    }
}

void ImGuiAssetProperties::RenderSprite(Sprite& sprite)
{
    std::string name = sprite.GetName() + " (Sprite)";
    ImGui::Text(name.c_str());

    std::string pathString = sprite.GetImage()->GetPath().string();

    Texture* texture = nullptr;
    AssetManager& assetManager = Project::GetActive()->GetAssetManager();

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