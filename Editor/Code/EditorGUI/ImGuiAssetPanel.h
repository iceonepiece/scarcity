#pragma once

#include <memory>
#include <filesystem>
#include <functional>
#include "Graphics/Texture.h"
#include "Graphics/Image.h"
#include "Asset/AudioAsset.h"
#include "Asset/PrefabAsset.h"
#include "Asset/NativeScriptAsset.h"

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

	void RenderPrefab(PrefabAsset& prefabAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	static void RenderImage(Image& image, ImGuiTreeNodeFlags flags, AssetEventFunction callback, OnSelectSpriteFunction selectSpriteFn = [](Sprite&, size_t){}, const std::string& note = "");
	static void RenderAudio(AudioAsset& audioAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	static void RenderAnimatorController(AnimatorControllerAsset& animControllerAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	static void RenderNativeScript(NativeScriptAsset& nativeScriptAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	static void RenderFolder(const std::filesystem::path& path, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	void RenderUnsupportedFile(const std::filesystem::path& path);

private:
	std::filesystem::path m_selectedPath;
	std::filesystem::path m_baseDirectory;
	std::filesystem::path m_currentDirectory;

	bool m_showDeleteModal = false;
	Asset* m_onActionAsset = nullptr;

	EditorLayer& m_editor;
};