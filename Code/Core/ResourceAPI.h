#pragma once

#include <string>
#include <memory>
#include "../Graphics/Texture.h"
#include "ResourceManager.h"

class ResourceAPI
{
public:
	static void Initialize(ResourceManager* resourceManager);
	static Texture* LoadTexture(std::string name, const char* filename, bool alpha = false);
	static void LoadParticles(std::string fileName);
	static bool HasTexture(const std::string& name);
	static Texture& GetTexture(const std::string& name);
	static ParticleProps GetParticle(const std::string& name);

private:
	static std::unique_ptr<ResourceManager> s_resourceManager;
};


