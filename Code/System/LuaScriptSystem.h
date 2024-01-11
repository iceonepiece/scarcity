#pragma once

#include "System.h"
#include "Lua/LuaEngine.h"

class LuaScriptSystem : public System
{
public:
    LuaScriptSystem(Scene& scene)
        : System(scene)
    {}

    virtual void Update(float deltaTime) override
    {
        LuaEngine& luaEngine = Application::Get().GetLuaEngine();

        auto luaScriptView = m_registry.view<LuaScriptComponent>();
        for (auto [entity, luaScript] : luaScriptView.each())
        {
            if (luaScript.script != nullptr && FileSystem::FileExists(luaScript.script->GetPath()))
            {
                luaEngine.ReadScript(luaScript.script->GetPath().string());
                sol::function updateFn = luaEngine.GetFunction("Update");
                updateFn(deltaTime);
            }
        }
    }
};