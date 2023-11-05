#include "GroundDetectionFixtureData.h"
#include "Components/GroundDetectionComponent.h"
#include <iostream>

GroundDetectionFixtureData::GroundDetectionFixtureData(Entity entity, GroundDetectionComponent& component)
    : FixtureData(entity, "GROUND_DETECTION")
    , m_groundDetection(component)
{
}

void GroundDetectionFixtureData::BeginContact(FixtureData* otherFixtureData, b2Contact* contact, bool isA)
{
    std::cout << "GroundDetectionFixtureData >> BeginContact\n";

    if (m_groundDetection.groundTag == otherFixtureData->m_tag)
    {
        b2WorldManifold worldManifold;
        contact->GetWorldManifold(&worldManifold);

        int yTarget = isA ? -1 : 1;

        if (worldManifold.normal.x == 0 && worldManifold.normal.y == yTarget)
        {
            m_groundDetection.numGrounds++;
            m_groundDetection.groundFixtures.emplace_back(otherFixtureData);
            std::cout << "Num Grounds: " << m_groundDetection.numGrounds << "\n";
        }
    }
}

void GroundDetectionFixtureData::EndContact(FixtureData* otherFixtureData, b2Contact* contact, bool isA)
{
    std::cout << "GroundDetectionFixtureData >> EndContact\n";

    auto iter = std::find(m_groundDetection.groundFixtures.begin(), m_groundDetection.groundFixtures.end(), otherFixtureData);

    if (iter != m_groundDetection.groundFixtures.end())
    {
        m_groundDetection.groundFixtures.erase(iter);
        m_groundDetection.numGrounds--;

        std::cout << "Num Grounds: " << m_groundDetection.numGrounds << "\n";
    }
}