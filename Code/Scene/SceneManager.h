#pragma once

#include <memory>
#include <filesystem>
#include "Scene/Scene.h"

class SceneManager
{
public:
	template<typename Component>
	static void CopyComponent(entt::registry& srcRegistry, entt::registry& destRegistry, entt::entity srcEntity, entt::entity destEntity)
	{
		if (srcRegistry.all_of<Component>(srcEntity)) {
			auto& component = srcRegistry.get<Component>(srcEntity);
			destRegistry.emplace_or_replace<Component>(destEntity, component);
		}
	}

	static std::unique_ptr<Scene> LoadScene(const std::filesystem::path& filePath);
	static std::unique_ptr<Scene> CreateDefaultScene(const std::filesystem::path& directory);
	static Scene* Copy(Scene& sourceScene);
};