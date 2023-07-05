#pragma once

#include <unordered_map>
#include <string>
#include <sol/sol.hpp>
#include <memory>

#include "Graphics/Texture.h"
#include "Core/ParticleSystem.h"
#include "Graphics/Shader.h"
#include "Core/ResourceManager.h"

class OpenGLResourceManager : public ResourceManager
{
public:
	virtual Texture* LoadTexture(std::string name, const char* filename, bool alpha = false) override;
	virtual void LoadParticles(std::string fileName) override;

};