#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct TransformComponent
{
	static std::string Name() { return "Transform"; }

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 world = glm::mat4(1.0f);

	TransformComponent(const glm::vec3& p, const glm::vec3& r, const glm::vec3& s)
		: position(p)
		, rotation(r)
		, scale(s)
	{
	}

	TransformComponent()
		: position(0)
		, rotation(0)
		, scale(1)
	{}

	glm::mat4 GetLocalMatrix() const
	{
		return glm::translate(glm::mat4(1.0f), position)
			* glm::mat4(glm::quat(rotation))
			* glm::scale(glm::mat4(1.0f), scale);
	}
};

static void DoSerialize(const TransformComponent& transform, json& entityJson)
{
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

static void DoDeserialize(TransformComponent& transform, json& transformJson)
{
	auto& positionJson = transformJson["position"];
	glm::vec3 position { positionJson[0].get<float>(), positionJson[1].get<float>(), positionJson[2].get<float>() };

	auto& rotationJson = transformJson["rotation"];
	glm::vec3 rotation { rotationJson[0].get<float>(), rotationJson[1].get<float>(), rotationJson[2].get<float>() };

	auto& scaleJson = transformJson["scale"];
	glm::vec3 scale { scaleJson[0].get<float>(), scaleJson[1].get<float>(), scaleJson[2].get<float>() };

	transform.position = position;
	transform.rotation = rotation;
	transform.scale = scale;
}