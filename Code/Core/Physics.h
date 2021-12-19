#pragma once

#include <box2d/box2d.h>

class Physics
{
public:
  Physics();
	void Update(float deltaTime);
  b2Body* CreateBoxBody(float x, float y, float width, float height, bool isDynamic = false);

private:
  b2World m_world;
  int32 m_velocityIterations;
  int32 m_positionIterations;
};
