#include "AnimationClip.h"
#include "Animations/AnimationSerializer.h"

AnimationClip::AnimationClip(const std::filesystem::path& path)
	: Asset(path, AssetType::AnimationClip)
	, m_image(nullptr)
{
	if (path != "")
		AnimationSerializer::Deserialize(*this, path);
}
