#pragma once

#include "System.h"
#include "Scene/Scene.h"

class UISystem : public System
{
public:
    UISystem(Scene& scene)
        : System(scene)
    {}

    virtual void OnStart() override
    {
    }

    virtual void OnStop() override
    {
    }

    virtual void Update(float deltaTime) override
    {
    }
};