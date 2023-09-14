#pragma once

#include "File/FileSystem.h"
#include "Asset/TextureAsset.h"
#include "Asset/PrefabAsset.h"

class ImGuiAssetProperties
{
public:
	void Render(Asset* asset);
	void RenderPrefabAsset(PrefabAsset& prefabAsset);
	void RenderTextureAsset(TextureAsset& textureAsset);
	void RenderSpriteAsset(SpriteAsset& spriteAsset);
};