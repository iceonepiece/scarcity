#pragma once

#include <unordered_map>
#include <string>
#include <sol/sol.hpp>

#include "ParticleSystem.h"
#include "../Rendering/Shader.h"

class ResourceManager
{
public:
	static std::unordered_map<std::string, ParticleProps> s_particles;

	static void LoadParticles(std::string fileName);

private:
	ResourceManager() {}
};