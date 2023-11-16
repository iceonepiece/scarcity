#pragma once

#include "Asset/TextureAsset.h"
#include "Asset/PrefabAsset.h"
#include "Asset/AudioAsset.h"

class ImGui_AssetInspector
{
public:
	static void Render(Asset* asset);
	static void RenderPrefabAsset(PrefabAsset& prefabAsset);
	static void RenderTextureAsset(TextureAsset& textureAsset);
	static void RenderSpriteAsset(SpriteAsset& spriteAsset);
	static void RenderAudioAsset(AudioAsset& audioAsset);
};