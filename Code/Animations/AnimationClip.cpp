#include "AnimationClip.h"

AnimationClip::AnimationClip(const std::filesystem::path& path)
	: Asset(path, AssetType::AnimationClip)
	, m_image(nullptr)
{

}
