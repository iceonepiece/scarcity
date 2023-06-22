#pragma once

#include <entt/entt.hpp>
#include "../Core/System.h"
#include "../Components/ScriptableComponent.h"

class ScriptableSystem : public System
{
public:
	ScriptableSystem(Scene *scene) : System(scene) {}

	virtual void Update(float deltaTime) override
	{
		auto view = m_registry.view<ScriptableComponent>();

		for (auto [entity, scriptable] : view.each())
		{
			scriptable.scriptable->Update(deltaTime);
		}
	}

	virtual void Render() override
	{

	}
};