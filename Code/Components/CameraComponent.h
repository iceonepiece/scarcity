#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class ProjectionType
{
	Perspective,
	Orthographic
};

struct CameraComponent
{
	static std::string Name() { return "Camera"; }

	ProjectionType projection = ProjectionType::Orthographic;
	float size = 1.0f;
	float near = 0.0f;
	float far = 1000.0f;
};

static void DoSerialize(const CameraComponent& camera, json& entityJson)
{
	entityJson["Camera"]["projection"] = camera.projection;
	entityJson["Camera"]["size"] = camera.size;
	entityJson["Camera"]["near"] = camera.near;
	entityJson["Camera"]["far"] = camera.far;
}

static void DoDeserialize(CameraComponent& camera, json& cameraJson)
{
	camera.projection = cameraJson["projection"].get<ProjectionType>();
	camera.size = cameraJson["size"].get<float>();
	camera.near = cameraJson["near"].get<float>();
	camera.far = cameraJson["far"].get<float>();
}