#pragma once

#include <memory>
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <filesystem>
#include "Asset/Asset.h"
#include "Animations/AnimatorController.h"
#include "Core/UniqueID.h"

class Scene;
class Image;

using AssetLinkFunction = std::function<void(Asset*)>;

struct AssetLink
{
	AssetLinkFunction linkFunction;
	UniqueID ID = 0;
};

class AssetManager
{
public:
	virtual ~AssetManager() = default;
	virtual void InitializeAssets(const std::filesystem::path& path);
	void LinkIDsToAssets();

	Scene* GetScene(const std::string& name);

	virtual Texture* LoadTexture(const std::string& name, const char* filename, bool alpha = false);
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

	std::vector<Image*>& GetImages() { return m_images; }
	std::vector<AnimationClip*>& GetAnimationClips() { return m_animClips; }
	std::vector<AnimatorController*>& GetAnimatorControllers() { return m_animControllers; }

	Asset* GetAssetByID(UniqueID id);
	void AddAssetLink(AssetLinkFunction fn, UniqueID id)
	{
		m_assetLinks.push_back({ fn, id });
	}

	AnimatorController* GetAnimatorController(const std::string& name);


protected:
	std::vector<AssetLink> m_assetLinks;

	std::map<std::string, std::unique_ptr<Scene>> m_sceneMap;
	std::map<std::string, std::unique_ptr<Asset>> m_assetMap;
	std::map<UniqueID, Asset*> m_assetIDMap;
	//std::map<std::string, SpriteAsset*> m_spriteAssetMap;
	std::map<std::string, Sprite*> m_spriteMap;
	std::map<std::string, std::unique_ptr<AnimatorController>> m_animControllerMap;

	std::vector<Image*> m_images;
	std::vector<AnimationClip*> m_animClips;
	std::vector<AnimatorController*> m_animControllers;

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
};