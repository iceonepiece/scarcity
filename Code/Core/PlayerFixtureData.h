#pragma once

#include "FixtureData.h"

class PlayerFixtureData : public FixtureData
{
public:
	PlayerFixtureData(Entity entity)
		: FixtureData(entity, "PLAYER")
	{
	}

	virtual void BeginContact(FixtureData* otherFixtureData, b2Contact* contact, bool isA)
	{
		b2WorldManifold worldManifold;
  	contact->GetWorldManifold(&worldManifold);

  	int yTarget = isA ? -1 : 1;

    if (worldManifold.normal.x == 0 && worldManifold.normal.y == yTarget)
  	{
			std::cout << "GROUNDED" << std::endl;
  		auto player = m_entity.GetComponent<PlayerComponent>();
  		player->numGrounds++;
  		player->groundFixtures.emplace_back(otherFixtureData);
  	}
	}

	virtual void EndContact(FixtureData* otherFixtureData, b2Contact* contact, bool isA)
	{
    auto player = m_entity.GetComponent<PlayerComponent>();
  	auto iter = std::find(player->groundFixtures.begin(), player->groundFixtures.end(), otherFixtureData);

    bool success = false;

  	if (iter != player->groundFixtures.end())
  	{
  		player->groundFixtures.erase(iter);
  		success = true;
  	}

  	if (success)
  	{
  		player->numGrounds--;
  	}
	}

  virtual void PreSolve(FixtureData* other, b2Contact* contact, const b2Manifold* oldManifold, bool isA)
	{
		if (other && other->m_tag == "ENEMY")
		{
			contact->SetEnabled(false);
		}
	}
};
