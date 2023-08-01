#pragma once

class Scene;

class GameState
{
public:
    GameState(Scene* scene);
    virtual void OnEnter() {}
    virtual void Process() {}
    virtual void OnExit() {}

protected:
    Scene* m_scene;
};