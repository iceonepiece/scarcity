#pragma once

#include "Graphics/Image.h"
#include "Graphics/Sprite.h"
#include "Entity/Prefab.h"
#include "Audio/AudioClip.h"
#include "Animations/AnimatorController.h"
#include "NativeScript/NativeScript.h"
#include "Lua/LuaScript.h"

class ImGui_AssetInspector
{
public:
	static void Render(Asset* asset);
	static void RenderPrefab(Prefab& prefab);
	static void RenderImage(Image& image);
	static void RenderSprite(Sprite& sprite);
	static void RenderAudioClip(AudioClip& audioClip);
	static void RenderAnimatorController(AnimatorController& animController);
	static void RenderAnimationClip(AnimationClip& animClip);
	static void RenderNativeScript(NativeScript& nativeScript);
	static void RenderLuaScript(LuaScript& luaScript);
};