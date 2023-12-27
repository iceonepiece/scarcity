#include "ImGui_AssetInspector.h"

#include <imgui/imgui.h>
#include "ImGui_AssetInspector.h"
#include "Graphics/Texture.h"
#include <iostream>
#include "File/MetaSerializer.h"
#include "Audio/AudioSource.h"
#include "Audio/Audio.h"
#include "Animations/AnimationClip.h"
#include "../ImGuiComponents/ImGuiComponentRenderer.h"

void ImGui_AssetInspector::Render(Asset* asset)
{
    if (Image* image = dynamic_cast<Image*>(asset))
        RenderImage(*image);

    else if (Sprite* sprite = dynamic_cast<Sprite*>(asset))
        RenderSprite(*sprite);

    else if (Prefab* prefab = dynamic_cast<Prefab*>(asset))
        RenderPrefab(*prefab);

    else if (AudioClip* audioClip = dynamic_cast<AudioClip*>(asset))
        RenderAudioClip(*audioClip);

    else if (AnimatorController* animatorAsset = dynamic_cast<AnimatorController*>(asset))
        RenderAnimatorController(*animatorAsset);

    else if (AnimationClip* animation = dynamic_cast<AnimationClip*>(asset))
        RenderAnimationClip(*animation);

    else if (NativeScript* nativeScript = dynamic_cast<NativeScript*>(asset))
		RenderNativeScript(*nativeScript);

    else if (LuaScript* luaScript = dynamic_cast<LuaScript*>(asset))
		RenderLuaScript(*luaScript);
}

void ImGui_AssetInspector::RenderNativeScript(NativeScript& nativeScript)
{
    std::string name = nativeScript.GetPath().stem().string() + " (Native Script)";

    ImGui::Text(name.c_str());
}

void ImGui_AssetInspector::RenderLuaScript(LuaScript& luaScript)
{
    std::string name = luaScript.GetPath().stem().string() + " (Lua Script)";

    ImGui::Text(name.c_str());
}

void ImGui_AssetInspector::RenderAnimatorController(AnimatorController& animController)
{
    std::string name = animController.GetPath().stem().string() + " (Animator Controller)";

    ImGui::Text(name.c_str());


    auto& parameters = animController.GetParameters();

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

void ImGui_AssetInspector::RenderAnimationClip(AnimationClip& animClip)
{
    std::string name = animClip.GetPath().stem().string() + " (Animation Clip)";

    static float frameTime = 0.1f;
    static float timer = 0.0f;
    static int currentFrame = 1;
    static Texture* currentTexture = nullptr;
    static bool isPlaying = false;

    ImGui::Text(name.c_str());

    ImGui::BeginChild("Preview", ImVec2(0, 0));

    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2{ 0.5f, 0.5f });

    std::vector<size_t> spriteIndices;
    std::vector<Sprite> sprites;

    if (animClip.GetImage() != nullptr)
    {
        currentTexture = animClip.GetImage()->GetTexture();
        spriteIndices = animClip.GetSpriteIndices();
        sprites = animClip.GetImage()->GetSprites();
    }

    ImGui::Text("Preview");
    ImGui::Text("Total Frames: %d", spriteIndices.size());

    ImGui::Text("Frame Time (Seconds): ");
    ImGui::SameLine();
    ImGui::InputFloat("##frameTime", &frameTime, 0);

    if (spriteIndices.size() > 0)
    {
        auto it = spriteIndices.begin();

        for (int i = 0; i < spriteIndices.size(); i++)
        {
            if (i == currentFrame - 1)
                break;
            it++;
        }

        if (it == spriteIndices.end())
        {
            currentFrame = 1;
            it = spriteIndices.begin();
        }

        auto& sprite = sprites[*it];

        if (currentTexture != nullptr)
        {
            const int SPRITE_WIDTH = 256;
            const int HALF_SPRITE_WIDTH = SPRITE_WIDTH / 2;

            int renderWidth = SPRITE_WIDTH;

            if (sprite.GetWidth() < HALF_SPRITE_WIDTH)
                renderWidth = HALF_SPRITE_WIDTH;
            else if (sprite.GetWidth() > SPRITE_WIDTH)
                renderWidth = SPRITE_WIDTH;

            ImVec2 renderSize{ (float)renderWidth, renderWidth * (sprite.GetRatio().y / sprite.GetRatio().x) };
            ImGui::Image((ImTextureID)currentTexture->GetRendererID(), renderSize, { sprite.GetLeft(), sprite.GetTop() }, { sprite.GetRight(), sprite.GetBottom() });
        }
    }

    if (isPlaying)
    {
        timer += ImGui::GetIO().DeltaTime;

        if (timer >= frameTime)
        {
            timer = 0.0f;
            currentFrame++;

            if (currentFrame > spriteIndices.size())
                currentFrame = 1;
        }

        if (ImGui::Button(ICON_FA_PAUSE))
            isPlaying = false;
    }
    else
    {
        if (ImGui::Button(ICON_FA_PLAY))
        {
            isPlaying = true;
            timer = 0.0f;
        }
    }


    ImGui::SameLine();

    if (ImGui::Button(ICON_FA_STOP))
    {
        isPlaying = false;
        currentFrame = 1;
        timer = 0.0f;
    }

    ImGui::SameLine();
    ImGui::SliderInt("Frame", &currentFrame, 1, spriteIndices.size());

    ImGui::PopStyleVar();

    ImGui::EndChild();
}

void ImGui_AssetInspector::RenderPrefab(Prefab& prefab)
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

void ImGui_AssetInspector::RenderImage(Image& image)
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

void ImGui_AssetInspector::RenderAudioClip(AudioClip& audioClip)
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

void ImGui_AssetInspector::RenderSprite(Sprite& sprite)
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

        ImVec2 renderSize{ (float)renderWidth, renderWidth * (sprite.GetRatio().y / sprite.GetRatio().x) };
        ImGui::Image((ImTextureID)texture->GetRendererID(), renderSize, { sprite.GetLeft(), sprite.GetTop() }, { sprite.GetRight(), sprite.GetBottom() });
    }
}