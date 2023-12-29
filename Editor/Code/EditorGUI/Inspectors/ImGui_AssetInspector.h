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
};