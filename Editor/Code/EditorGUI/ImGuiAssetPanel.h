#pragma once

#include <memory>
#include <filesystem>
#include <functional>
#include "Graphics/Texture.h"
#include "Graphics/Image.h"
#include "Audio/AudioClip.h"
#include "Entity/Prefab.h"
#include "NativeScript/NativeScript.h"
#include "Lua/LuaScript.h"
#include "Animations/AnimatorController.h"

class EditorLayer;

using AssetEventFunction = std::function<void()>;
//using OnSelectSpriteFunction = std::function<void(SpriteAsset&)>;
using OnSelectSpriteFunction = std::function<void(Sprite&, size_t)>;

class ImGuiAssetPanel
{
public:
	ImGuiAssetPanel(EditorLayer& editor);
	void Render();

	void SetProjectDirectory(std::filesystem::path path)
	{
		m_baseDirectory = path;
		m_currentDirectory = path;
	}

	void RenderPrefab(Prefab& prefab, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	static void RenderImage(Image& image, ImGuiTreeNodeFlags flags, AssetEventFunction callback, OnSelectSpriteFunction selectSpriteFn = [](Sprite&, size_t){}, const std::string& note = "");
	static void RenderAudioClip(AudioClip& audioClip, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	void RenderAnimatorController(AnimatorController& animControllerAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	void RenderAnimationClip(AnimationClip& animClip, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	static void RenderNativeScript(NativeScript& nativeScript, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	static void RenderLuaScript(LuaScript& luaScript, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	static void RenderFolder(const std::filesystem::path& path, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	void RenderUnsupportedFile(const std::filesystem::path& path);

private:
	void RightClickMenu(Asset& asset);
	void RenderAddAssetButton();

	std::filesystem::path m_selectedPath;
	std::filesystem::path m_baseDirectory;
	std::filesystem::path m_currentDirectory;

	AssetType m_addingAssetType = AssetType::None;
	std::string m_addingAssetName = "";

	bool m_showDeleteModal = false;
	Asset* m_onActionAsset = nullptr;

	EditorLayer& m_editor;
};