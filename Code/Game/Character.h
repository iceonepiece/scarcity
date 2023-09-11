#pragma once

#include "Scene/ScriptableEntity.h"

class Character : public ScriptableEntity
{
public:
	virtual void Update(float dt) override;

};