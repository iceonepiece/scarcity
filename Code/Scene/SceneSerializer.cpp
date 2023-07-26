#pragma once

#include "SceneSerializer.h"
#include <string>
#include <fstream>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "Components/Components.h"

using json = nlohmann::json;

void SceneSerializer::Serialize(const std::string& filepath)
{
	std::ofstream sceneFile;
	sceneFile.open(filepath);

	if (sceneFile.is_open())
	{
		json sceneJson;
		sceneJson["name"] = "Untitled";
		sceneJson["entities"] = json::array();

		auto& registry = m_scene.m_manager.m_registry;

		registry.each([&](entt::entity entity) {

			json entityJson = {};
			entityJson["id"] = entity;

			if (registry.all_of<BaseComponent>(entity))
			{
				auto& base = registry.get<BaseComponent>(entity);
				entityJson["Base"]["name"] = base.name;
			}

			if (registry.all_of<TransformComponent>(entity))
			{
				auto& transform = registry.get<TransformComponent>(entity);

				json positionJson = json::array();
				positionJson.push_back(transform.position.x);
				positionJson.push_back(transform.position.y);
				positionJson.push_back(transform.position.z);
				entityJson["Transform"]["position"] = positionJson;

				json rotationJson = json::array();
				rotationJson.push_back(transform.rotation.x);
				rotationJson.push_back(transform.rotation.y);
				rotationJson.push_back(transform.rotation.z);
				entityJson["Transform"]["rotation"] = rotationJson;

				json scaleJson = json::array();
				scaleJson.push_back(transform.scale.x);
				scaleJson.push_back(transform.scale.y);
				scaleJson.push_back(transform.scale.z);
				entityJson["Transform"]["scale"] = scaleJson;
			}

			if (registry.all_of<SpriteRendererComponent>(entity))
			{
				auto& sprite = registry.get<SpriteRendererComponent>(entity);

				entityJson["SpriteRenderer"]["shape"] = sprite.shape;

				json colorJson = json::array();
				colorJson.push_back(sprite.color.x);
				colorJson.push_back(sprite.color.y);
				colorJson.push_back(sprite.color.z);
				colorJson.push_back(sprite.color.w);
				entityJson["SpriteRenderer"]["color"] = colorJson;
			}

			sceneJson["entities"].push_back(entityJson);
			});

		std::cout << sceneJson << std::endl;
		sceneFile << sceneJson.dump(4);
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return;
	}

	sceneFile.close();
}

SceneSerializer::SceneSerializer(Scene& scene)
	: m_scene(scene)
{

}

bool SceneSerializer::Deserialize(const std::string& filepath)
{
	std::ifstream sceneFile(filepath);
	
	if (sceneFile.is_open())
	{
		EntityManager& manager = m_scene.GetEntityManager();

		json sceneJson = json::parse(sceneFile);
		
		json entitiesJson = sceneJson["entities"];

		for (auto& entityJson : entitiesJson)
		{
			Entity entity = manager.CreateEntity();

			if (entityJson["Base"].is_object())
			{
				json baseJson = entityJson["Base"];
				entity.AddComponent<BaseComponent>(baseJson["name"]);
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
		}
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return false;
	}

	sceneFile.close();

	return true;
}
