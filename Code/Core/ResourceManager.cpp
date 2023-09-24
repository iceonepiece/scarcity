#include "Core/ResourceManager.h"
#include "Asset/TextureAsset.h"
#include "Asset/PrefabAsset.h"

void ResourceManager::InitializeAssets(const std::filesystem::path& path)
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

			if (directoryEntry.is_directory())
			{
				std::cout << "[Folder]";
				pathQueue.push(targetPath);
			}
			else if (FileSystem::IsSceneFile(targetPath))
			{
				std::cout << "[Scene]";
				std::unique_ptr<Scene> scene = std::make_unique<Scene>(targetPath.stem().string(), targetPath);
				m_sceneMap.insert({ targetPath.stem().string(),std::move(scene) });

				std::cout << "Initialize Scene: " << targetPath.stem().string();
			}
			else if (FileSystem::IsImageFile(targetPath))
			{
				std::cout << "[Texture]";
				FileSystem::HandleMetaFile(targetPath);
				std::unique_ptr<TextureAsset> textureAsset = std::make_unique<TextureAsset>(targetPath);
				m_assetMap.insert({ targetPath.string(), std::move(textureAsset) });
			}
			else if (FileSystem::IsAnimatorFile(targetPath))
			{
				std::cout << "[Animator Controller]";
				m_animControllerMap.insert({ targetPath.string(), std::make_unique<AnimatorControllerAsset>(targetPath) });
			}
			else
			{
				std::cout << "[Not supported]";
			}

			std::cout << "\t" << targetPath << std::endl;
		}

		pathQueue.pop();
	}

}