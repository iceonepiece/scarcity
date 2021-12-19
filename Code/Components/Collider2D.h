#pragma once

struct Collider2D
{
	b2Body* body;

	Collider2D(b2Body* body) : body(body) {}
};
