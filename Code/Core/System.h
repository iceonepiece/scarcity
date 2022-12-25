#pragma once

#include "Scene.h"

class System
{
public:
	System(Scene *scene)
		: scene(scene)
	{
	}

	virtual void ProcessInput(entt::registry& registry) = 0;
	virtual void Update(float deltaTime, entt::registry& registry) = 0;

	bool active = true;

protected:
	Scene *scene;
};