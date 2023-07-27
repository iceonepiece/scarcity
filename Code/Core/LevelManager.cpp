#include "LevelManager.h"
#include "../Components/Collider2DComponent.h"

void LevelManager::LoadLevel(Scene* scene, std::string filePath)
{
    sol::state luaState;
    luaState.script_file(filePath);

    sol::table platforms = luaState["platforms"];
    for (int i = 0;; i++)
    {
        sol::optional<sol::table> isExist = platforms[i];

        if (isExist == sol::nullopt)
            break;

        sol::table node = platforms[i];

        auto platform = scene->m_manager.CreateEntity();
        b2Vec2 position { node["x"], node["y"]};
        b2Vec2 size { node["width"], node["height"] };
        //b2Body* body = scene->m_physics->CreateBodyWithFixture(position, size, new FixtureData(platform, "PLATFORM"), false, false, PhysicsLayer::Layer_Platform);
        //platform.AddComponent<Collider2DComponent>(body);
    }
}