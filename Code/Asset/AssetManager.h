#pragma once

#include <memory>
#include <unordered_map>
#include <map>
#include <string>
#include <filesystem>
#include "Asset/Asset.h"
#include "Animations/AnimatorController.h"

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

	Sprite* GetSprite(const std::string& name);
	void AddSprites(std::vector<Sprite>& sprites);
	void RemoveSprites(std::vector<Sprite>& sprites);

	Asset* GetAssetByID(UniqueID id);

	//SpriteAsset* GetSpriteAsset(const std::string& name);
	//void AddSpriteAssets(std::vector<SpriteAsset>& spriteAssets);
	//void RemoveSpriteAssets(std::vector<SpriteAsset>& spriteAssets);

	AnimatorController* GetAnimatorController(const std::string& name);
	std::map<std::string, std::unique_ptr<AnimatorController>>& GetAnimatorControllers();


protected:
	std::map<std::string, std::unique_ptr<Scene>> m_sceneMap;
	std::map<std::string, std::unique_ptr<Asset>> m_assetMap;
	std::map<UniqueID, Asset*> m_assetIDMap;
	//std::map<std::string, SpriteAsset*> m_spriteAssetMap;
	std::map<std::string, Sprite*> m_spriteMap;
	std::map<std::string, std::unique_ptr<AnimatorController>> m_animControllerMap;

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
};