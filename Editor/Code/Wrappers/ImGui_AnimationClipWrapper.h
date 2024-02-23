#pragma once

#include "Animations/AnimationClip.h"
#include "ImGui_AssetWrapper.h"

class ImGui_AnimationClipWrapper : public ImGui_AssetWrapper
{
public:
    virtual void RenderBrowser(EditorLayer& editor) override;

	virtual void RenderInspector()
	{
        std::string name = m_animationClip->GetPath().stem().string() + " (Animation Clip)";

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

        if (m_animationClip->GetImage() != nullptr)
        {
            currentTexture = m_animationClip->GetImage()->GetTexture();
            spriteIndices = m_animationClip->GetSpriteIndices();
            sprites = m_animationClip->GetImage()->GetSprites();
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

	virtual void SetAsset(Asset* asset)
    {
		m_animationClip = static_cast<AnimationClip*>(asset);
    }

private:
	AnimationClip* m_animationClip;

};