#pragma once

#include <box2d/b2_body.h>

struct CircleCollider2DComponent
{
	b2Body* body;

	CircleCollider2DComponent(b2Body* body) : body(body) {}
};
