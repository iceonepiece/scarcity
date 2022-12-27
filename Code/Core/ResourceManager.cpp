#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::unordered_map<std::string, Texture> ResourceManager::s_textures;
std::unordered_map<std::string, ParticleProps> ResourceManager::s_particles;

Texture ResourceManager::LoadTexture(std::string name, const char* filename, bool alpha)
{
    Texture texture;
    texture.Generate(filename, alpha);

    s_textures.emplace(name, texture);

    return s_textures[name];
}

void ResourceManager::LoadParticles(std::string fileName)
{
    sol::state luaState;
    luaState.script_file(fileName);

    sol::table particles = luaState["particles"];
    for (int i = 0;; i++)
    {
        sol::optional<sol::table> isExist = particles[i];

        if (isExist == sol::nullopt)
            break;

        sol::table node = particles[i];

        ParticleProps particleProps;
        particleProps.amount = node["amount"];

        particleProps.colorBegin = {
            node["colorBegin"]["r"],
            node["colorBegin"]["g"],
            node["colorBegin"]["b"],
            node["colorBegin"]["a"]
        };

        particleProps.colorEnd = {
            node["colorEnd"]["r"],
            node["colorEnd"]["g"],
            node["colorEnd"]["b"],
            node["colorEnd"]["a"]
        };

        particleProps.sizeBegin = node["sizeBegin"];
        particleProps.sizeVariation = node["sizeVariation"];
        particleProps.sizeEnd = node["sizeEnd"];
        particleProps.lifeTime = node["lifeTime"];
        particleProps.velocity = { node["velocity"]["x"], node["velocity"]["y"] };
        particleProps.velocityVariation = { node["velocityVariation"]["x"], node["velocityVariation"]["y"] };
        particleProps.position = { node["position"]["x"], node["position"]["y"] };

        s_particles.emplace(node["name"], particleProps);
    }
}