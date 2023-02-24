#pragma once

#include <unordered_map>
#include <string>
#include <sol/sol.hpp>

#include "Texture.h"
#include "ParticleSystem.h"
#include "../Graphics/Shader.h"

class ResourceManager
{
public:
	static std::unordered_map<std::string, Texture> s_textures;
	static std::unordered_map<std::string, ParticleProps> s_particles;

	static Texture LoadTexture(std::string name, const char* filename, bool alpha = false);
	static void LoadParticles(std::string fileName);


private:
	ResourceManager() { std::cout << "ResourceManager constructor" << std::endl; }
};