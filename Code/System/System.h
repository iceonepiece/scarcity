#pragma once

#include <entt/entt.hpp>

class Scene;

class System
{
public:
	System(Scene& scene);
	virtual ~System() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render() {}

	bool IsActive() const
	{
		return m_active;
	}

protected:
	bool m_active;
	Scene& m_scene;
	entt::registry& m_registry;
};