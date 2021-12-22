#pragma once

struct Collider2DComponent
{
	b2Body* body;

	Collider2DComponent(b2Body* body) : body(body) {}
};
