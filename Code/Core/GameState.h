#pragma once

#include "Scene.h"

class GameState
{
public:
    GameState(Scene * scene)
        : m_scene(scene)
    {
    }

    virtual void OnEnter()
    {

    }

    virtual void Process()
    {

    }

    virtual void OnExit()
    {

    }

protected:
    Scene* m_scene;
};