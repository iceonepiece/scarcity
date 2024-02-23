#pragma once

#include <filesystem>
#include "Asset/Asset.h"

class AudioClip : public Asset
{
public:
	AudioClip(const std::filesystem::path& path)
		: Asset(path, AssetType::AudioClip)
		, m_duration(0)
	{
	}

	virtual bool Load() = 0;

	virtual ~AudioClip() = default;

	inline float GetDuration()
	{
		return m_duration;
	}

protected:
	float m_duration;
};