#pragma once

#include "File/FileSystem.h"
#include "Graphics/Sprite.h"

class ImGuiResourceProperties
{
public:
	void Render(Resource* resource);

private:
	SpriteResource m_currentSpriteResource;
};