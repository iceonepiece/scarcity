#pragma once

#include <memory>
#include <filesystem>
#include <functional>
#include "Graphics/Texture.h"
#include "Asset/TextureAsset.h"
#include "Asset/PrefabAsset.h"
#include "Asset/NativeScriptAsset.h"

class EditorLayer;

using AssetEventFunction = std::function<void()>;
using OnSelectSpriteFunction = std::function<void(SpriteAsset&)>;

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

	static void RenderPrefab(PrefabAsset& prefabAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	static void RenderTexture(TextureAsset& textureAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback, OnSelectSpriteFunction selectSpriteFn = [](SpriteAsset&){}, const std::string& note = "");
	static void RenderNativeScript(NativeScriptAsset& nativeScriptAsset, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	static void RenderFolder(const std::filesystem::path& path, ImGuiTreeNodeFlags flags, AssetEventFunction callback);
	void RenderUnsupportedFile(const std::filesystem::path& path);

private:
	std::filesystem::path m_selectedPath;
	std::filesystem::path m_baseDirectory;
	std::filesystem::path m_currentDirectory;

	std::unique_ptr<Texture> m_folderIcon;
	std::unique_ptr<Texture> m_fileIcon;

	EditorLayer& m_editor;
};