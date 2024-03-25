#pragma once

#include "System.h"
#include "Components/TilemapComponent.h"
#include "Graphics/Renderer.h"
#include "Core/Application.h"

class TilemapSystem : public System
{
public:
    TilemapSystem(Scene& scene)
        : System(scene)
    {}

    virtual void Update(float dt) override
    {
    }

    virtual void Render() override
    {
        Renderer& renderer = Application::Get().GetRenderer();
        auto tilemapView = m_registry.view<TilemapComponent>();

        for (auto [entity, tilemap] : tilemapView.each())
        {
            if (tilemap.originalImage == nullptr)
                continue;

            if (Texture* texture = tilemap.originalImage->GetTexture())
            {
                for (auto& [pos, value] : tilemap.data)
                {
                    int row = tilemap.rows - value.first - 1;

                    int spriteIndex = row * tilemap.cols;
                    spriteIndex += value.second;

                    if (spriteIndex < tilemap.tilemapSprites.size())
                    {
                        renderer.DrawSprite(tilemap.tilemapSprites[spriteIndex], { pos.first + 0.5f, pos.second + 0.5f }, { 1, 1 }, 0.0f);
                    }
                }
            }
        }
    }
};