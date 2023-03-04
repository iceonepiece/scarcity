#pragma once

#include <box2d/box2d.h>
#include "FixtureData.h"

class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
    {
        FixtureData* fixtureDataA = FixtureData::GetFixtureData(contact->GetFixtureA());
        FixtureData* fixtureDataB = FixtureData::GetFixtureData(contact->GetFixtureB());

        if (fixtureDataA) fixtureDataA->BeginContact(fixtureDataB, contact, true);
        if (fixtureDataB) fixtureDataB->BeginContact(fixtureDataA, contact, false);
	}

	void EndContact(b2Contact* contact)
    {
        FixtureData* fixtureDataA = FixtureData::GetFixtureData(contact->GetFixtureA());
        FixtureData* fixtureDataB = FixtureData::GetFixtureData(contact->GetFixtureB());

        if (fixtureDataA) fixtureDataA->EndContact(fixtureDataB, contact, true);
        if (fixtureDataB) fixtureDataB->EndContact(fixtureDataA, contact, false);
	}

    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
    {
        FixtureData* fixtureDataA = FixtureData::GetFixtureData(contact->GetFixtureA());
        FixtureData* fixtureDataB = FixtureData::GetFixtureData(contact->GetFixtureB());

        if (fixtureDataA) fixtureDataA->PreSolve(fixtureDataB, contact, oldManifold, true);
        if (fixtureDataB) fixtureDataB->PreSolve(fixtureDataA, contact, oldManifold, false);
    }
};
