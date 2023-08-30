#include "ResourceAPI.h"
#include <iostream>

std::unique_ptr<ResourceManager> ResourceAPI::s_resourceManager = nullptr;

void ResourceAPI::Initialize(ResourceManager* resourceManager)
{
	s_resourceManager = std::unique_ptr<ResourceManager>(resourceManager);
}

Texture* ResourceAPI::LoadTexture(std::string name, const char* filename, bool alpha)
{
	return s_resourceManager->LoadTexture(name, filename, alpha);
}

void ResourceAPI::LoadParticles(std::string fileName)
{
	s_resourceManager->LoadParticles(fileName);
}

bool ResourceAPI::HasTexture(const std::string& name)
{
	return s_resourceManager->HasTexture(name);
}

Texture& ResourceAPI::GetTexture(const std::string& name)
{
	return s_resourceManager->GetTexture(name);
}

ParticleProps ResourceAPI::GetParticle(const std::string& name)
{
	return s_resourceManager->GetParticle(name);
}