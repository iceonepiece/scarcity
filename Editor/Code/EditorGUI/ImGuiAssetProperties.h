#pragma once

#include "File/FileSystem.h"
#include "Asset/TextureAsset.h"

class ImGuiAssetProperties
{
public:
	void Render(Asset* asset);
	void RenderTextureAsset(TextureAsset& textureAsset);
	void RenderSpriteAsset(SpriteAsset& spriteAsset);
};