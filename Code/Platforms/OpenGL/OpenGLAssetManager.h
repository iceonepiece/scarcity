#pragma once

#include <string>

#include "Graphics/Texture.h"
#include "Asset/AssetManager.h"

class OpenGLAssetManager : public AssetManager
{
public:
	virtual Texture* LoadTexture(const std::string& name, const char* filename, bool alpha = false) override;
};