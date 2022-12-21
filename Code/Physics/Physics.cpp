#include "Physics.h"

Physics::Physics()
	: m_world(b2Vec2(0.0f, -18.0f))
	, m_velocityIterations(6)
	, m_positionIterations(2)
{
  m_world.SetContactListener(&m_contactListener);
}

Physics::~Physics()
{
  for (auto fixtureData : m_fixtureDatum)
  {
    delete fixtureData;
  }
}

void Physics::Update(float deltaTime) 
{
	m_world.Step(deltaTime, m_velocityIterations, m_positionIterations);
}


void Physics::CreateFixtureDef(b2Body* body, b2Vec2 size, b2Vec2 offset, bool isSensor, FixtureData* fixtureData)
{
  b2PolygonShape shape;
	shape.SetAsBox(size.x / 2, size.y / 2, offset, 0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = isSensor ? 0.0f : 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = isSensor;

  m_fixtureDatum.push_back(fixtureData);
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);

	body->CreateFixture(&fixtureDef);
}

b2Body* Physics::CreateBodyWithFixture(b2Vec2 position, b2Vec2 size, FixtureData* fixtureData, bool isDynamic, bool isSensor)
{
  b2BodyDef bodyDef;
	bodyDef.position.Set(position.x, position.y);

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
  body->SetFixedRotation(true);

  b2PolygonShape shape;
  shape.SetAsBox(size.x / 2, size.y / 2);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shape;
  fixtureDef.density = isSensor ? 0.0f : 1.0f;
  fixtureDef.friction = 0.0f;
  fixtureDef.isSensor = isSensor;

  if (fixtureData)
  {
    m_fixtureDatum.push_back(fixtureData);
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
  }

  body->CreateFixture(&fixtureDef);

  return body;
}

b2Body* Physics::CreateBoxBody(float x, float y, float width, float height, bool isDynamic, bool isSensor, float gravityScale)
{
  b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);

	if (isDynamic)
	{
		bodyDef.type = b2_dynamicBody;
		bodyDef.gravityScale = gravityScale;
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
  fixtureDef.isSensor = isSensor;

  body->SetFixedRotation(true);
	body->CreateFixture(&fixtureDef);

  return body;
}
