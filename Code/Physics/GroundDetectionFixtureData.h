#pragma once

#include "FixtureData.h"

class GroundDetectionComponent;

class GroundDetectionFixtureData : public FixtureData
{
public:
    GroundDetectionFixtureData(Entity entity, GroundDetectionComponent& component);
    virtual void BeginContact(FixtureData* otherFixtureData, b2Contact* contact, bool isA) override;
    virtual void EndContact(FixtureData* otherFixtureData, b2Contact* contact, bool isA) override;

private:
    GroundDetectionComponent& m_groundDetection;
};
