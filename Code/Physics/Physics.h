#pragma once

#include <iostream>
#include <set>
#include <vector>
#include <box2d/box2d.h>
#include "ContactListener.h"

class Physics
{
public:
  Physics();
  ~Physics();

  void Update(float deltaTime);
  b2Body* CreateBodyWithFixture(b2Vec2 position, b2Vec2 size, FixtureData* fixtureData, bool isDynamic = false, bool isSensor = false);
  b2Body* CreateBoxBody(float x, float y, float width, float height, bool isDynamic = false, bool isSensor = false, float gravityScale = 1.0f);
  void CreateFixtureDef(b2Body* body, b2Vec2 size, b2Vec2 offset, bool isSensor, FixtureData* fixtureData);

private:
  b2World m_world;
  ContactListener m_contactListener;
  int32 m_velocityIterations;
  int32 m_positionIterations;

  std::vector<FixtureData*> m_fixtureDatum;
};
