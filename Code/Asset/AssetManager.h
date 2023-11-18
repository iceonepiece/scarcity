#pragma once

#include <memory>
#include <unordered_map>
#include <map>
#include <string>
#include <filesystem>
#include "Asset/Asset.h"
#include "Asset/AnimatorControllerAsset.h"
#include "Asset/TextureAsset.h"

class Scene;

class AssetManager
{
public:
	void InitializeAssets(const std::filesystem::path& path);

	Scene* GetScene(const std::string& name);

	virtual Texture* LoadTexture(const std::string& name, const char* filename, bool alpha = false) = 0;
	void RemoveTexture(const std::string& name);

	bool HasTexture(const std::string& name);
	Texture& GetTexture(const std::string& name);
	Texture* GetTexturePtr(const std::string& name);

	Asset* GetAsset(const std::filesystem::path& path, bool loadIfNotExist = false);
	Asset* LoadAsset(const std::filesystem::path& path);
	bool AssetExists(const std::filesystem::path& path);

	SpriteAsset* GetSpriteAsset(const std::string& name);
	void AddSpriteAssets(std::vector<SpriteAsset>& spriteAssets);
	void RemoveSpriteAssets(std::vector<SpriteAsset>& spriteAssets);

	AnimatorController* GetAnimatorController(const std::string& name);
	std::map<std::string, std::unique_ptr<AnimatorControllerAsset>>& GetAnimatorControllerAssets();


protected:
	std::map<std::string, std::unique_ptr<Scene>> m_sceneMap;
	std::map<std::string, std::unique_ptr<Asset>> m_assetMap;
	std::map<std::string, SpriteAsset*> m_spriteAssetMap;
	std::map<std::string, std::unique_ptr<AnimatorControllerAsset>> m_animControllerMap;

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
};