#pragma once

#include "File/FileSystem.h"
#include "Graphics/Image.h"
#include "Graphics/Sprite.h"
#include "Asset/PrefabAsset.h"
#include "Asset/AudioAsset.h"

class ImGuiAssetProperties
{
public:
	void Render(Asset* asset);
	void RenderPrefabAsset(PrefabAsset& prefabAsset);
	void RenderImage(Image& image);
	void RenderSprite(Sprite& sprite);
	void RenderAudioAsset(AudioAsset& audioAsset);
};