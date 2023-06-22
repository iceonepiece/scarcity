#pragma once

#include <unordered_map>
#include <string>
#include <sol/sol.hpp>
#include <memory>

#include "../Graphics/Texture.h"
#include "ParticleSystem.h"
#include "../Graphics/Shader.h"

class ResourceManager
{
public:
	virtual Texture* LoadTexture(std::string name, const char* filename, bool alpha = false) = 0;
	virtual void LoadParticles(std::string fileName) = 0;

	
	Texture& GetTexture(const std::string& name)
	{
		return *m_textures[name];
	}

	ParticleProps GetParticle(const std::string& name)
	{
		return m_particles[name];
	}

protected:
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
	std::unordered_map<std::string, ParticleProps> m_particles;
};