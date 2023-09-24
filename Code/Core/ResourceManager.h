#pragma once

#include <unordered_map>
#include <map>
#include <queue>
#include <string>
#include <memory>
#include <filesystem>
#include <sol/sol.hpp>
#include <entt/entt.hpp>

#include "File/FileSystem.h"
#include "../Graphics/Texture.h"
#include "ParticleSystem.h"
#include "../Graphics/Shader.h"
#include "Animations/Sprite.h"
#include "Asset/Asset.h"
#include "Asset/AnimatorControllerAsset.h"
#include "Asset/TextureAsset.h"
#include "Scene/Scene.h"

class ResourceManager
{
public:
	virtual Texture* LoadTexture(std::string name, const char* filename, bool alpha = false) = 0;
	virtual void LoadParticles(std::string fileName) = 0;

	void InitializeAssets(const std::filesystem::path& path);

	void AddSpriteAssets(std::vector<SpriteAsset>& spriteAssets)
	{
		for (auto& sprite : spriteAssets)
		{
			std::cout << sprite.GetSprite().GetName() << " Added\n";
			m_spriteAssetMap.insert({ sprite.GetSprite().GetName(), &sprite });
		}
	}

	void RemoveSpriteAssets(std::vector<SpriteAsset>& spriteAssets)
	{
		for (auto& sprite : spriteAssets)
			m_spriteAssetMap.erase(sprite.GetSprite().GetName());
	}

	SpriteAsset* GetSpriteAsset(const std::string& name)
	{
		return m_spriteAssetMap.find(name) != m_spriteAssetMap.end() ? m_spriteAssetMap[name] : nullptr;
	}

	bool HasTexture(const std::string& name)
	{
		return m_textures.find(name) != m_textures.end();
	}

	void RemoveTexture(const std::string& name)
	{
		m_textures.erase(name);
	}

	Scene* GetScene(const std::string& name)
	{
		if (m_sceneMap.find(name) != m_sceneMap.end())
			return m_sceneMap[name].get();

		return nullptr;
	}
	
	Texture& GetTexture(const std::string& name)
	{
		return *m_textures[name];
	}

	ParticleProps GetParticle(const std::string& name)
	{
		return m_particles[name];
	}

	inline std::map<std::string, SpriteAsset*>& GetSpriteMap() { return m_spriteAssetMap; }
	inline AnimatorController* GetAnimatorController(const std::string& name)
	{
		if (m_animControllerMap.find(name) != m_animControllerMap.end())
			return m_animControllerMap[name]->GetController();

		return nullptr;
	}

	inline std::map<std::string, std::unique_ptr<AnimatorControllerAsset>>& GetAnimatorControllerAssets()
	{
		return m_animControllerMap;
	}


protected:
	//entt::registry m_prefabRegistry;
	EntityManager m_prefabManager;

	std::map<std::string, std::unique_ptr<Scene>> m_sceneMap;
	std::map<std::string, std::unique_ptr<Asset>> m_assetMap;
	std::map<std::string, SpriteAsset*> m_spriteAssetMap;
	std::map<std::string, std::unique_ptr<AnimatorControllerAsset>> m_animControllerMap;

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
	std::unordered_map<std::string, ParticleProps> m_particles;
};