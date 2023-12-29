#pragma once

#include "ImGui_AssetWrapper.h"
#include "Graphics/Sprite.h"
#include "Graphics/Image.h"
#include "Project/Project.h"

class ImGui_SpriteWrapper : public ImGui_AssetWrapper
{
public:
    virtual void RenderBrowser()
    {

    }

	virtual void RenderInspector() override
	{
        std::string name = m_sprite->GetName() + " (Sprite)";
        ImGui::Text(name.c_str());

        std::string pathString = m_sprite->GetImage()->GetPath().string();

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
            ImGui::Text("size = %d x %d", (int)m_sprite->GetWidth(), (int)m_sprite->GetHeight());

            int renderWidth = 512;

            if (m_sprite->GetWidth() < 256)
                renderWidth = 256;
            else if (m_sprite->GetWidth() > 512)
                renderWidth = 512;

            ImVec2 renderSize{ (float)renderWidth, renderWidth * (m_sprite->GetRatio().y / m_sprite->GetRatio().x) };
            ImGui::Image((ImTextureID)texture->GetRendererID(), renderSize, { m_sprite->GetLeft(), m_sprite->GetTop() }, { m_sprite->GetRight(), m_sprite->GetBottom() });
        }
	}

	virtual void SetAsset(Asset* asset)
	{
		m_sprite = dynamic_cast<Sprite*>(asset);
	}

private:
	Sprite* m_sprite = nullptr;
};