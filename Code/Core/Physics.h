#pragma once

#include <iostream>
#include <set>
#include <vector>
#include <box2d/box2d.h>

typedef std::pair<b2Fixture*, b2Fixture*> fixturePair;

class ContactListener : public b2ContactListener
{
public:
  std::set<fixturePair> m_fixturePairs;

  void BeginContact(b2Contact* contact)
  {
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    if (fixtureA->IsSensor() && fixtureB->GetBody()->GetType() == b2_dynamicBody)
      m_fixturePairs.insert(std::make_pair(fixtureA, fixtureB));
    else if (fixtureB->IsSensor() && fixtureA->GetBody()->GetType() == b2_dynamicBody)
      m_fixturePairs.insert(std::make_pair(fixtureB, fixtureA));
	}

	void EndContact(b2Contact* contact)
  {
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    if (fixtureA->IsSensor() && fixtureB->GetBody()->GetType() == b2_dynamicBody)
      m_fixturePairs.erase(std::make_pair(fixtureA, fixtureB));
    else if (fixtureB->IsSensor() && fixtureA->GetBody()->GetType() == b2_dynamicBody)
      m_fixturePairs.erase(std::make_pair(fixtureB, fixtureA));
	}
};

class Physics
{
public:
  Physics();
	void Update(float deltaTime);
  b2Body* CreateBoxBody(float x, float y, float width, float height, bool isDynamic = false, bool isSensor = false);

private:
  b2World m_world;
  ContactListener m_contactListener;
  int32 m_velocityIterations;
  int32 m_positionIterations;
};
