#pragma once

#include <string>
#include <box2d/box2d.h>
#include "../Core/Entity.h"

class FixtureData
{
public:
	static FixtureData* GetFixtureData(b2Fixture* fixture)
	{
	  b2FixtureUserData fixtureUserData = fixture->GetUserData();
	  FixtureData* fixtureData = (FixtureData*)fixtureUserData.pointer;
	  return fixtureData;
	}

	FixtureData(Entity entity, std::string tag = "")
    : m_entity(entity)
    , m_tag(tag)
  {
  }

	virtual ~FixtureData()
	{
	}

	virtual void BeginContact(FixtureData* other, b2Contact* contact, bool isA) {}
	virtual void EndContact(FixtureData* other, b2Contact* contact, bool isA) {}
	virtual void PreSolve(FixtureData* other, b2Contact* contact, const b2Manifold* oldManifold, bool isA) {}

	std::string m_tag;
	Entity m_entity;
};
