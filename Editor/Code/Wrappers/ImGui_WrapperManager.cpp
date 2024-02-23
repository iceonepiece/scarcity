#include "ImGui_WrapperManager.h"

#include "ImGui_AudioClipWrapper.h"
#include "ImGui_ImageWrapper.h"
#include "ImGui_NativeScriptWrapper.h"
#include "ImGui_PrefabWrapper.h"
#include "ImGui_SceneWrapper.h"
#include "ImGui_SpriteWrapper.h"
#include "ImGui_AnimatorControllerWrapper.h"
#include "ImGui_AudioClipWrapper.h"
#include "ImGui_AnimationClipWrapper.h"
#include "ImGui_LuaScriptWrapper.h"

/*
	None,
	Scene,
	NativeScript,
	Prefab,
	Image,
	Sprite,
	AnimatorController,
	AudioClip,
	AnimationClip,
	LuaScript,
*/

std::unique_ptr<ImGui_AssetWrapper> ImGui_WrapperManager::s_assetWrappers[(size_t)AssetType::Count]
{
	std::make_unique<ImGui_AssetWrapper>(),
	std::make_unique<ImGui_SceneWrapper>(),
	std::make_unique<ImGui_NativeScriptWrapper>(),
	std::make_unique<ImGui_PrefabWrapper>(),
	std::make_unique<ImGui_ImageWrapper>(),
	std::make_unique<ImGui_SpriteWrapper>(),
	std::make_unique<ImGui_AnimatorControllerWrapper>(),
	std::make_unique<ImGui_AudioClipWrapper>(),
	std::make_unique<ImGui_AnimationClipWrapper>(),
	std::make_unique<ImGui_LuaScriptWrapper>(),
};

ImGui_AssetWrapper* ImGui_WrapperManager::GetWrapper(Asset& asset)
{
	ImGui_AssetWrapper* wrapper = s_assetWrappers[(size_t)asset.GetType()].get();
	wrapper->SetAsset(&asset);

	return wrapper;
}