#pragma once

#include <unordered_map>
#include <map>
#include <queue>
#include <string>
#include <memory>
#include <filesystem>
#include <sol/sol.hpp>

#include "File/FileSystem.h"
#include "../Graphics/Texture.h"
#include "ParticleSystem.h"
#include "../Graphics/Shader.h"
#include "Animations/Sprite.h"
#include "Asset/Asset.h"
#include "Asset/AnimatorControllerAsset.h"

class ResourceManager
{
public:
	virtual Texture* LoadTexture(std::string name, const char* filename, bool alpha = false) = 0;
	virtual void LoadParticles(std::string fileName) = 0;

	void InitializeAssets(const std::filesystem::path& path);

	void AddSprites(std::vector<Sprite>& sprites)
	{
		for (auto& sprite : sprites)
		{
			std::cout << sprite.GetName() << " Added\n";
			m_spriteMap.insert({ sprite.GetName(), &sprite });
		}
	}

	void RemoveSprites(std::vector<Sprite>& sprites)
	{
		for (auto& sprite : sprites)
			m_spriteMap.erase(sprite.GetName());
	}

	Sprite* GetSprite(const std::string& name)
	{
		return m_spriteMap.find(name) != m_spriteMap.end() ? m_spriteMap[name] : nullptr;
	}

	bool HasTexture(const std::string& name)
	{
		return m_textures.find(name) != m_textures.end();
	}

	void RemoveTexture(const std::string& name)
	{
		m_textures.erase(name);
	}
	
	Texture& GetTexture(const std::string& name)
	{
		return *m_textures[name];
	}

	ParticleProps GetParticle(const std::string& name)
	{
		return m_particles[name];
	}

	inline std::map<std::string, Sprite*>& GetSpriteMap() { return m_spriteMap; }
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
	std::map<std::string, std::unique_ptr<Asset>> m_assetMap;
	std::map<std::string, Sprite*> m_spriteMap;
	std::map<std::string, std::unique_ptr<AnimatorControllerAsset>> m_animControllerMap;

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
	std::unordered_map<std::string, ParticleProps> m_particles;
};