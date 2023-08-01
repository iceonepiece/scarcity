#pragma once

#include <entt/entt.hpp>

class Scene;

class System
{
public:
	System(Scene* scene);
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	bool active = true;

protected:
	Scene *m_scene;
	entt::registry& m_registry;
};