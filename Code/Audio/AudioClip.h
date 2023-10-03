#pragma once

#include <filesystem>

class AudioClip
{
public:
	AudioClip(const std::filesystem::path& filePath)
		: m_filePath(filePath)
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
	std::filesystem::path m_filePath;
	float m_duration;
};