#pragma once

#include "Asset/Asset.h"
#include "Graphics/Image.h"

struct AnimationClip : public Asset
{
public:
	AnimationClip(const std::filesystem::path& path);

private:
	Image* m_image;
	std::vector<std::string> m_spriteNames;

};