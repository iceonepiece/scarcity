#include "AssetManager.h"
#include "Asset/TextureAsset.h"
#include "Asset/AudioAsset.h"
#include "Asset/PrefabAsset.h"
#include "Asset/NativeScriptAsset.h"
#include "Core/Application.h"

Asset* AssetManager::LoadAsset(const std::filesystem::path& path)
{
    if (FileSystem::IsImageFile(path))
    {
        std::cout << "LoadAsset (Texture): " << path << "\n";
        std::unique_ptr<TextureAsset> textureAsset = std::make_unique<TextureAsset>(path);
        m_assetMap.insert({ path.string(), std::move(textureAsset) });
    }
    else if (FileSystem::IsAudioFile(path))
    {
        std::cout << "LoadAsset (Audio): " << path << "\n";
        std::unique_ptr<AudioAsset> audioAsset = std::make_unique<AudioAsset>(path);
        m_assetMap.insert({ path.string(), std::move(audioAsset) });
    }
    else if (FileSystem::IsPrefabFile(path))
    {
        std::cout << "LoadAsset (Prefab): " << path << "\n";
        Entity entity = Application::Get().GetPrefabManager().CreateEntity();
        std::unique_ptr<PrefabAsset> prefabAsset = std::make_unique<PrefabAsset>(path, entity);
        m_assetMap.insert({ path.string(), std::move(prefabAsset) });
    }
    else if (FileSystem::IsNativeScriptFile(path))
    {
        std::cout << "LoadAsset (NativeScrpt): " << path << "\n";
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
    std::cout << "AssetManager::GetAsset: " << path << std::endl;
    if (AssetExists(path))
        return m_assetMap[path.string()].get();

    if (loadIfNotExist)
        return LoadAsset(path);

    return nullptr;
}