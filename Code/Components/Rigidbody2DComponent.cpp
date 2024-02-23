#include "Rigidbody2DComponent.h"

void* Rigidbody2DComponent::AddFixture(Physics& physics, TransformComponent transform, Collider2DData data, Entity entity)
{
	if (data.type == ColliderType::Box)
	{
		BoxCollider2DComponent bc2d;
		bc2d.isTrigger = data.isTrigger;
		bc2d.size = data.size;
		bc2d.offset = data.offset;

		b2PolygonShape boxShape = Physics::CreateBoxShape(transform, bc2d);
		b2FixtureDef fixtureDef = physics.CreateFixtureDef(boxShape, bc2d, data.layer);
		FixtureData* fixtureData = Physics::CreateFixtureData(entity);

		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
		this->fixtureData = fixtureData;

		return ((b2Body*)body)->CreateFixture(&fixtureDef);
	}
	else if (data.type == ColliderType::Circle)
	{
		CircleCollider2DComponent cc2d;
		cc2d.isTrigger = data.isTrigger;
		cc2d.radius = data.size.x;
		cc2d.offset = data.offset;

		b2CircleShape circleShape = Physics::CreateCircleShape(transform, cc2d);
		b2FixtureDef fixtureDef = physics.CreateFixtureDef(circleShape, cc2d, data.layer);
		FixtureData* fixtureData = Physics::CreateFixtureData(entity);

		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
		this->fixtureData = fixtureData;

		return ((b2Body*)body)->CreateFixture(&fixtureDef);
	}

	return nullptr;
}