#pragma once

#include "System.h"
#include "Components/NativeScriptComponent.h"

class NativeScriptSystem : public System
{
public:
    NativeScriptSystem(Scene& scene)
        : System(scene)
    {}

	virtual void Update(float deltaTime) override
	{
        auto nativeScriptUpdateView = m_registry.view<NativeScriptComponent>();
        for (auto [entity, nativeScript] : nativeScriptUpdateView.each())
        {
            if (nativeScript.instance != nullptr)
                nativeScript.instance->Update(deltaTime);
        }
	}
};