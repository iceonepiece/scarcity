#pragma once

#include <string>
#include <box2d/box2d.h>
#include "../Core/Entity.h"
#include "FixtureData.h"
#include "Scene/ScriptableEntity.h"
#include "Components/NativeScriptComponent.h"
#include "Components/BaseComponent.h"

class EntityFixtureData : public FixtureData
{
public:
	EntityFixtureData(Entity entity)
		: FixtureData(entity, "ENTITY")
	{
	}

	virtual void BeginContact(FixtureData* other, b2Contact* contact, bool isA)
	{
		if (auto groundDetection = m_entity.GetComponent<GroundDetectionComponent>())
		{
			std::cout << "GroundDetectionFixtureData >> BeginContact\n";

			if (groundDetection->groundTag == other->m_tag)
			{
				b2WorldManifold worldManifold;
				contact->GetWorldManifold(&worldManifold);

				int yTarget = isA ? -1 : 1;

				if (worldManifold.normal.x == 0 && worldManifold.normal.y == yTarget)
				{
					groundDetection->numGrounds++;
					groundDetection->groundFixtures.emplace_back(other);
					std::cout << "Num Grounds: " << groundDetection->numGrounds << "\n";
				}
			}
		}

		if (auto nativeScript = m_entity.GetComponent<NativeScriptComponent>())
		{
			if (nativeScript->instance)
			{
				BaseComponent* base = other->m_entity.GetComponent<BaseComponent>();
				if (base)
					nativeScript->instance->OnCollisionEnter({ base->name, base->tag });
			}
		}
	}

	virtual void EndContact(FixtureData* other, b2Contact* contact, bool isA)
	{
		if (auto groundDetection = m_entity.GetComponent<GroundDetectionComponent>())
		{
			std::cout << "GroundDetectionFixtureData >> EndContact\n";

			auto iter = std::find(groundDetection->groundFixtures.begin(), groundDetection->groundFixtures.end(), other);

			if (iter != groundDetection->groundFixtures.end())
			{
				groundDetection->groundFixtures.erase(iter);
				groundDetection->numGrounds--;

				std::cout << "Num Grounds: " << groundDetection->numGrounds << "\n";
			}
		}

		if (auto nativeScript = m_entity.GetComponent<NativeScriptComponent>())
		{
			if (nativeScript->instance)
			{
				BaseComponent* base = other->m_entity.GetComponent<BaseComponent>();
				if (base)
					nativeScript->instance->OnCollisionExit({ base->name, base->tag });
			}
		}
	}

	virtual void PreSolve(FixtureData* other, b2Contact* contact, const b2Manifold* oldManifold, bool isA) {}
};
