#pragma once

#include "Scene/Scene.h"

class System
{
public:
	System(Scene *scene)
		: m_scene(scene)
		, m_registry(scene->GetEntityManager().m_registry)
	{
	}

	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	bool active = true;

protected:
	Scene *m_scene;
	entt::registry& m_registry;
};