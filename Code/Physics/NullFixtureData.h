#pragma once

#include "FixtureData.h"

class NullFixtureData : public FixtureData
{
public:
	NullFixtureData(Entity entity)
		: FixtureData(entity, "NULL")
	{
	}
};