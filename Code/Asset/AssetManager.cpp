#include "AssetManager.h"
#include "Graphics/Image.h"
#include "Audio/Audio.h"
#include "Audio/AudioClip.h"
#include "Entity/Prefab.h"
#include "Scene/Scene.h"
#include "Lua/LuaScript.h"
#include "NativeScript/NativeScript.h"
#include "Core/Application.h"
#include "Graphics/Renderer.h"
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

	LinkIDsToAssets();
}

void AssetManager::LinkIDsToAssets()
{
	for (auto& link : m_assetLinks)
	{
		if (Asset* asset = GetAssetByID(link.ID))
		{
			link.linkFunction(asset);
		}
	}
}

Texture* AssetManager::LoadTexture(const std::string& name, const char* filename, bool alpha)
{
	std::unique_ptr<Texture> texture = Application::Get().GetRenderer().LoadTexture(name, filename, alpha);

	if (texture != nullptr)
	{
		m_textures.emplace(name, std::move(texture));
		return m_textures[name].get();
	}

	return nullptr;
}

Scene* AssetManager::GetScene(const std::string& name)
{
	if (m_sceneMap.find(name) != m_sceneMap.end())
		return m_sceneMap[name];

	return nullptr;
}

Asset* AssetManager::LoadAsset(const std::filesystem::path& path)
{
    if (FileSystem::IsImageFile(path))
    {
        std::cout << "[Image] : " << path << '\n';
        std::unique_ptr<Image> image = std::make_unique<Image>(path);
        m_assetMap.insert({ path.string(), std::move(image) });

		if (m_assetMap.find(path.string()) != m_assetMap.end())
		{
			if (Image* imagePtr = dynamic_cast<Image*>(m_assetMap[path.string()].get()))
			{
				m_images.push_back(imagePtr);
				m_assetIDMap.insert({ imagePtr->GetID(), imagePtr });
			}
		}
    }
	else if (FileSystem::IsSceneFile(path))
	{
		std::cout << "[Scene] : " << path << '\n';
		std::unique_ptr<Scene> scene = std::make_unique<Scene>(path.stem().string(), path);
		//m_sceneMap.insert({ path.stem().string(),std::move(scene) });
		m_assetMap.insert({ path.string(), std::move(scene) });

		if (Scene* myScene = dynamic_cast<Scene*>(m_assetMap[path.string()].get()))
		{
			m_sceneMap.insert({ path.stem().string(), myScene });
		}
	}
	else if (FileSystem::IsAnimatorFile(path))
	{
		std::cout << "[Animator Controller] : " << path << '\n';
		m_assetMap.insert({ path.string(), std::make_unique<AnimatorController>(path) });

		if (m_assetMap.find(path.string()) != m_assetMap.end())
		{
			if (AnimatorController* controllerPtr = dynamic_cast<AnimatorController*>(m_assetMap[path.string()].get()))
			{
				controllerPtr->DoDeserialize(false);
				m_animControllers.push_back(controllerPtr);
				m_assetIDMap.insert({ controllerPtr->GetID(), controllerPtr });
			}
		}
	}
	else if (FileSystem::IsAnimationFile(path))
	{
		std::cout << "[Animation Clip] : " << path << '\n';
		m_assetMap.insert({ path.string(), std::make_unique<AnimationClip>(path) });

		if (m_assetMap.find(path.string()) != m_assetMap.end())
		{
			if (AnimationClip* clipPtr = dynamic_cast<AnimationClip*>(m_assetMap[path.string()].get()))
			{
				m_animClips.push_back(clipPtr);
				m_assetIDMap.insert({ clipPtr->GetID(), clipPtr });
			}
		}
	}
    else if (FileSystem::IsAudioFile(path))
    {
        std::cout << "[Audio] : " << path << '\n';
		std::unique_ptr<AudioClip> audioClip(Audio::Get()->LoadAudioClip(path));
        m_assetMap.insert({ path.string(), std::move(audioClip) });
    }
    else if (FileSystem::IsPrefabFile(path))
    {
        std::cout << "[Prefab] : " << path << '\n';
		Entity entity = Project::GetActive()->GetPrefabManager().CreateEntity();
		std::unique_ptr<Prefab> prefab = std::make_unique<Prefab>(path, entity);
		Project::GetActive()->AddPrefab(entity);
        m_assetMap.insert({ path.string(), std::move(prefab) });
    }
    else if (FileSystem::IsNativeScriptFile(path))
    {
        std::cout << "[NativeScrpt] : " << path << '\n';
        std::unique_ptr<NativeScript> nativeScritpAsset = std::make_unique<NativeScript>(path);
        m_assetMap.insert({ path.string(), std::move(nativeScritpAsset) });
    }
	else if (FileSystem::IsLuaScriptFile(path))
	{
		std::cout << "[LuaScript] : " << path << '\n';
		std::unique_ptr<LuaScript> luaScript = std::make_unique<LuaScript>(path);
		m_assetMap.insert({ path.string(), std::move(luaScript) });
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

Asset* AssetManager::GetAssetByID(UniqueID id)
{
	if (m_assetIDMap.find(id) != m_assetIDMap.end())
		return m_assetIDMap[id];

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

Texture* AssetManager::GetTexturePtr(const std::string& name)
{
	if (m_textures.find(name) != m_textures.end())
		return m_textures[name].get();

	return nullptr;
}

void AssetManager::RemoveTexture(const std::string& name)
{
	m_textures.erase(name);
}

Sprite* AssetManager::GetSprite(const std::string& name)
{
	return m_spriteMap.find(name) != m_spriteMap.end() ? m_spriteMap[name] : nullptr;
}

void AssetManager::AddSprites(std::vector<Sprite>& sprites)
{
	for (auto& sprite : sprites)
	{
		m_spriteMap.insert({ sprite.GetName(), &sprite });
	}
}

void AssetManager::RemoveSprites(std::vector<Sprite>& sprites)
{
	for (auto& sprite : sprites)
		m_spriteMap.erase(sprite.GetName());
}

AnimatorController* AssetManager::GetAnimatorController(const std::string& name)
{
	for (auto& controller : m_animControllers)
	{
		if (controller->GetName() == name)
			return controller;
	}

    return nullptr;
}