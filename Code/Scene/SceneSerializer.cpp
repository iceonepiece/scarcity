#pragma once

#include "SceneSerializer.h"
#include "Components/ComponentSerializer.h"
#include <string>
#include <fstream>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "Components/Components.h"

using json = nlohmann::json;

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

			/*
			if (entityJson["Base"].is_object())
			{
				BaseComponent base;
				json baseJson = entityJson["Base"];
				entity.AddComponent<BaseComponent>(transform);
			}

			if (entityJson["Transform"].is_object())
			{
				json transformJson = entityJson["Transform"];

				auto& positionJson = transformJson["position"];
				glm::vec3 position { positionJson[0].get<float>(), positionJson[1].get<float>(), positionJson[2].get<float>() };
	
				auto& rotationJson = transformJson["rotation"];
				glm::vec3 rotation { rotationJson[0].get<float>(), rotationJson[1].get<float>(), rotationJson[2].get<float>() };

				auto& scaleJson = transformJson["scale"];
				glm::vec3 scale { scaleJson[0].get<float>(), scaleJson[1].get<float>(), scaleJson[2].get<float>() };

				entity.AddComponent<TransformComponent>(
					position,
					rotation,
					scale
				);
			}

			if (entityJson["SpriteRenderer"].is_object())
			{
				json spriteRendererJson = entityJson["SpriteRenderer"];

				auto& colorJson = spriteRendererJson["color"];
				glm::vec4 color { colorJson[0].get<float>(), colorJson[1].get<float>(), colorJson[2].get<float>(), colorJson[3].get<float>() };

				entity.AddComponent<SpriteRendererComponent>(Shape_Square, color);
			}
			*/
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
