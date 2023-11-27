#pragma once

#include "Graphics/Image.h"
#include "Graphics/Sprite.h"
#include "Asset/PrefabAsset.h"
#include "Asset/AudioAsset.h"
#include "Animations/AnimatorController.h"

class ImGui_AssetInspector
{
public:
	static void Render(Asset* asset);
	static void RenderPrefabAsset(PrefabAsset& prefabAsset);
	static void RenderImage(Image& image);
	static void RenderSprite(Sprite& sprite);
	static void RenderAudioAsset(AudioAsset& audioAsset);
	static void RenderAnimatorController(AnimatorController& animController);
	static void RenderAnimationClip(AnimationClip& animClip);
};