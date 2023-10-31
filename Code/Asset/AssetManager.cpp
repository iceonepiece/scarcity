#include "AssetManager.h"
#include "Asset/TextureAsset.h"
#include "Asset/AudioAsset.h"
#include "Asset/PrefabAsset.h"
#include "Scene/Scene.h"
#include "Asset/NativeScriptAsset.h"
#include "Core/Application.h"
#include <filesystem>
#include <queue>

void AssetManager::InitializeAssets(const std::filesystem::path& path)
{
	std::queue<std::filesystem::path> pathQueue;

	std::cout << "Initialize Assets path: " << path << std::endl;

	pathQueue.push(path);

	while (!pathQueue.empty())
	{
		const std::filesystem::path& folderPath = pathQueue.front();

		for (auto& directoryEntry : std::filesystem::directory_iterator(folderPath))
		{
			std::filesystem::path targetPath = directoryEntry.path();

			if (directoryEntry.is_directory() && !FileSystem::IsIgnoreDirectory(targetPath))
			{
				std::cout << "[Folder] : " << targetPath << '\n';
				pathQueue.push(targetPath);
			}
			else
			{
				Asset* asset = LoadAsset(targetPath);
				if (asset == nullptr)
					std::cout << "[Not supported] : " << targetPath << '\n';
			}
		}

		pathQueue.pop();
	}
}

Scene* AssetManager::GetScene(const std::string& name)
{
	if (m_sceneMap.find(name) != m_sceneMap.end())
		return m_sceneMap[name].get();

	return nullptr;
}

Asset* AssetManager::LoadAsset(const std::filesystem::path& path)
{
    if (FileSystem::IsImageFile(path))
    {
        std::cout << "[Texture] : " << path << '\n';
        std::unique_ptr<TextureAsset> textureAsset = std::make_unique<TextureAsset>(path);
        m_assetMap.insert({ path.string(), std::move(textureAsset) });
    }
	else if (FileSystem::IsSceneFile(path))
	{
		std::cout << "[Scene] : " << path << '\n';
		std::unique_ptr<Scene> scene = std::make_unique<Scene>(path.stem().string(), path);
		m_sceneMap.insert({ path.stem().string(),std::move(scene) });
	}
	else if (FileSystem::IsAnimatorFile(path))
	{
		std::cout << "[Animator] : " << path << '\n';
		m_animControllerMap.insert({ path.string(), std::make_unique<AnimatorControllerAsset>(path) });
	}
    else if (FileSystem::IsAudioFile(path))
    {
        std::cout << "[Audio] : " << path << '\n';
        std::unique_ptr<AudioAsset> audioAsset = std::make_unique<AudioAsset>(path);
        m_assetMap.insert({ path.string(), std::move(audioAsset) });
    }
    else if (FileSystem::IsPrefabFile(path))
    {
        std::cout << "[Prefab] : " << path << '\n';
		Entity entity = Application::Get().GetPrefabManager().CreateEntity();
		std::unique_ptr<PrefabAsset> prefabAsset = std::make_unique<PrefabAsset>(path, entity);
		Application::Get().AddPrefab(entity);
        m_assetMap.insert({ path.string(), std::move(prefabAsset) });
    }
    else if (FileSystem::IsNativeScriptFile(path))
    {
        std::cout << "[NativeScrpt] : " << path << '\n';
        std::unique_ptr<NativeScriptAsset> nativeScritpAsset = std::make_unique<NativeScriptAsset>(path);
        m_assetMap.insert({ path.string(), std::move(nativeScritpAsset) });
    }

    return GetAsset(path);
}

bool AssetManager::AssetExists(const std::filesystem::path& path)
{
	return m_assetMap.find(path.string()) != m_assetMap.end();
}

Asset* AssetManager::GetAsset(const std::filesystem::path& path, bool loadIfNotExist)
{
    if (AssetExists(path))
        return m_assetMap[path.string()].get();

    if (loadIfNotExist)
        return LoadAsset(path);

    return nullptr;
}

bool AssetManager::HasTexture(const std::string& name)
{
	return m_textures.find(name) != m_textures.end();
}

Texture& AssetManager::GetTexture(const std::string& name)
{
	return *m_textures[name];
}

void AssetManager::RemoveTexture(const std::string& name)
{
	m_textures.erase(name);
}

SpriteAsset* AssetManager::GetSpriteAsset(const std::string& name)
{
	return m_spriteAssetMap.find(name) != m_spriteAssetMap.end() ? m_spriteAssetMap[name] : nullptr;
}

void AssetManager::AddSpriteAssets(std::vector<SpriteAsset>& spriteAssets)
{
	for (auto& sprite : spriteAssets)
	{
		m_spriteAssetMap.insert({ sprite.GetSprite().GetName(), &sprite });
	}
}

void AssetManager::RemoveSpriteAssets(std::vector<SpriteAsset>& spriteAssets)
{
	for (auto& sprite : spriteAssets)
		m_spriteAssetMap.erase(sprite.GetSprite().GetName());
}

AnimatorController* AssetManager::GetAnimatorController(const std::string& name)
{
    if (m_animControllerMap.find(name) != m_animControllerMap.end())
        return m_animControllerMap[name]->GetController();

    return nullptr;
}

std::map<std::string, std::unique_ptr<AnimatorControllerAsset>>& AssetManager::GetAnimatorControllerAssets()
{
	return m_animControllerMap;
}