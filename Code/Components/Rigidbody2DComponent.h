#pragma once

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
	BodyType type = BodyType::Static;
	bool fixedRotation = false;
	void* body = nullptr;
};