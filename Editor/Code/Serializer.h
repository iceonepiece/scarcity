#pragma once

#include <string>
#include <fstream>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Serializer
{
public:
	static void SerializeScene(entt::registry& registry, const std::string& sceneName = "Untitled")
	{
		std::ofstream sceneFile;
		sceneFile.open("Scenes/" + sceneName + ".scene.json");

		if (sceneFile.is_open())
		{
			json sceneJson;
			sceneJson["name"] = sceneName;
			sceneJson["entities"] = json::array();

			registry.each([&](entt::entity entity) {

				json entityJson = {};
				entityJson["id"] = entity;

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
		}

		sceneFile.close();
	}

	static void SerializeEntity(entt::entity entity)
	{

	}
};