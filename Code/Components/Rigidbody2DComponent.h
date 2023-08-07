#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class BodyType
{
	Static,
	Dynamic,
	Kinematic
};

inline b2BodyType Rigidbody2DTypeToBox2DBody(BodyType bodyType)
{
	switch (bodyType)
	{
		case BodyType::Static:    return b2_staticBody;
		case BodyType::Dynamic:   return b2_dynamicBody;
		case BodyType::Kinematic: return b2_kinematicBody;
	}

	return b2_staticBody;
}

struct Rigidbody2DComponent
{
	static std::string Name() { return "Rigidbody2D"; }

	BodyType type = BodyType::Static;
	bool fixedRotation = false;
	void* body = nullptr;
};

static void DoSerialize(const Rigidbody2DComponent& rb2d, json& entityJson)
{
	entityJson["Rigidbody2D"]["type"] = rb2d.type;
	entityJson["Rigidbody2D"]["fixedRotation"] = rb2d.fixedRotation;
}

static void DoDeserialize(Rigidbody2DComponent& rb2d, json& rb2dJson)
{
	rb2d.type = rb2dJson["type"].get<BodyType>();
	rb2d.fixedRotation = rb2dJson["fixedRotation"].get<bool>();
}