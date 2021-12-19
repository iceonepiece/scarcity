#include "Physics.h"

Physics::Physics()
  : m_world(b2Vec2(0.0f, 25.0f))
  , m_velocityIterations(6)
  , m_positionIterations(2)
{
}

void Physics::Update(float deltaTime)
{
	m_world.Step(deltaTime, m_velocityIterations, m_positionIterations);
}

b2Body* Physics::CreateBoxBody(float x, float y, float width, float height, bool isDynamic)
{
  b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);

	if (isDynamic)
	{
		bodyDef.type = b2_dynamicBody;
		bodyDef.gravityScale = 1.0f;
	}
	else
	{
		bodyDef.type = b2_staticBody;
	}

	b2Body* body = m_world.CreateBody(&bodyDef);

  b2PolygonShape shape;
	shape.SetAsBox(width / 2, height / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;

	body->CreateFixture(&fixtureDef);

  return body;
}
