#pragma once

enum class BodyType
{
	Static,
	Dynamic,
	Kinematic
};

struct Rigidbody2DComponent
{
	BodyType type = BodyType::Static;
	bool fixedRotation = false;
	void* body = nullptr;
};