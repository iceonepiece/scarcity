#pragma once

#include "File/FileSystem.h"
#include "Graphics/Image.h"
#include "Graphics/Sprite.h"
#include "Entity/Prefab.h"
#include "Audio/AudioClip.h"

class ImGuiAssetProperties
{
public:
	void Render(Asset* asset);
	void RenderPrefab(Prefab& prefab);
	void RenderImage(Image& image);
	void RenderSprite(Sprite& sprite);
	void RenderAudioClip(AudioClip& audioClip);
};