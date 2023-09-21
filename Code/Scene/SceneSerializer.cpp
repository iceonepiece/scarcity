#pragma once

#include "SceneSerializer.h"
#include "Components/ComponentSerializer.h"
#include <string>
#include <fstream>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "Components/Components.h"

using json = nlohmann::json;

void SceneSerializer::SerializeEntity(Scene& scene, entt::entity entity, std::filesystem::path filePath)
{
	auto& registry = scene.m_manager.m_registry;

	BaseComponent* base = registry.try_get<BaseComponent>(entity);

	if (base == nullptr)
		return;

	FileSystem::WriteFile(filePath / (base->name + ".prefab"), [&](std::fstream& fs)
	{
		ComponentSerializer serializer(registry);

		json entityJson = {};
		entityJson["id"] = entity;

		std::apply([&](auto... componentTypes) {
			(serializer.Serialize<decltype(componentTypes)>(entityJson, entity), ...);
		}, ComponentList{});

		fs << entityJson.dump(4);
	});
}


bool SceneSerializer::Serialize(Scene& scene, std::filesystem::path filePath)
{
	std::ofstream sceneFile;

	sceneFile.open(filePath);

	if (sceneFile.is_open())
	{
		json sceneJson;
		sceneJson["name"] = scene.m_name;
		sceneJson["entities"] = json::array();

		auto& registry = scene.m_manager.m_registry;
		ComponentSerializer serializer(registry);

		registry.each([&](entt::entity entity) {
			json entityJson = {};
			entityJson["id"] = entity;

			std::apply([&](auto... componentTypes) {
				(serializer.Serialize<decltype(componentTypes)>(entityJson, entity), ...);
			}, ComponentList{});	
			
			sceneJson["entities"].push_back(entityJson);
		});

		std::cout << sceneJson << std::endl;
		sceneFile << sceneJson.dump(4);
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return false;
	}

	sceneFile.close();

	return true;
}

bool SceneSerializer::Deserialize(Scene& scene, std::filesystem::path filepath)
{
	std::ifstream sceneFile(filepath);
	
	if (sceneFile.is_open())
	{
		scene.m_name = filepath.stem().string();
		scene.m_location = filepath.parent_path();
		scene.m_path = filepath;

		EntityManager& manager = scene.GetEntityManager();
		ComponentSerializer serializer(manager.m_registry);

		json sceneJson = json::parse(sceneFile);
		
		json entitiesJson = sceneJson["entities"];

		for (auto& entityJson : entitiesJson)
		{
			Entity entity = manager.CreateEntity();

			std::apply([&](auto... componentTypes) {
				(serializer.Deserialize<decltype(componentTypes)>(entityJson, entity), ...);
			}, ComponentList{});
		}
	}
	else
	{
		std::cerr << "Error opening the file! - " << filepath << std::endl;
		return false;
	}

	std::cout << "Deserialize scene at path: " << filepath << std::endl;

	sceneFile.close();

	return true;
}
