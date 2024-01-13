#pragma once

#include <string>
#include <algorithm>
#include <box2d/box2d.h>
#include "../Entity/Entity.h"
#include "FixtureData.h"
#include "Entity/ScriptableEntity.h"
#include "Components/NativeScriptComponent.h"
#include "Components/BaseComponent.h"
#include "Components/GroundDetectionComponent.h"

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
					groundDetection->groundContacts.emplace_back(contact);
					groundDetection->numGrounds++;

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
				{
					Collision2D collsion2D{ base->name, base->tag, contact->GetFixtureB()->GetFilterData().categoryBits };

					if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
						nativeScript->instance->OnTriggerEnter(collsion2D);
					else
						nativeScript->instance->OnCollisionEnter(collsion2D);
				}
			}
		}
	}

	virtual void EndContact(FixtureData* other, b2Contact* contact, bool isA)
	{
		if (auto groundDetection = m_entity.GetComponent<GroundDetectionComponent>())
		{
			std::cout << "GroundDetectionFixtureData >> EndContact\n";

			auto iter = std::find(groundDetection->groundContacts.begin(), groundDetection->groundContacts.end(), contact);

			if (iter != groundDetection->groundContacts.end())
			{
				groundDetection->groundContacts.erase(iter);
				groundDetection->numGrounds--;

				std::cout << "Num Grounds: " << groundDetection->numGrounds << "\n";
			}
		}

		if (auto nativeScript = m_entity.GetComponent<NativeScriptComponent>())
		{
			if (nativeScript->instance)
			{
				BaseComponent* base = other->m_entity.GetComponent<BaseComponent>();

				Collision2D collsion2D{ base->name, base->tag, contact->GetFixtureB()->GetFilterData().categoryBits };

				if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor())
					nativeScript->instance->OnTriggerExit(collsion2D);
				else
					nativeScript->instance->OnCollisionExit(collsion2D);
			}
		}
	}

	virtual void PreSolve(FixtureData* other, b2Contact* contact, const b2Manifold* oldManifold, bool isA) {}
};
