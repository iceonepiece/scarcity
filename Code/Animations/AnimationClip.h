#pragma once

#include "Asset/Asset.h"
#include "Graphics/Image.h"

struct AnimationClip : public Asset
{
public:
	AnimationClip(const std::filesystem::path& path);

	void SetImage(Image* image) { m_image = image; }
	Image* GetImage() { return m_image; }
	std::vector<size_t>& GetSpriteIndices() { return m_spriteIndices; }

private:
	Image* m_image;
	std::vector<std::string> m_spriteNames;
	std::vector<size_t> m_spriteIndices;

	friend class AnimationSerializer;
};