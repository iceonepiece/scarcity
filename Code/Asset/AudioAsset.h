#pragma once

#include "Asset.h"
#include "Audio/Audio.h"
#include "Core/Application.h"

class AudioAsset : public Asset
{
public:
	AudioAsset(const std::filesystem::path& path)
		: Asset(path, AssetType::Audio)
	{
		Audio& audio = Application::Get().GetAudio();
		audio.LoadSound(m_path.string(), m_path.string());
		m_audioSource = audio.GetAudioSource(m_path.string());
	}

	AudioSource* GetAudioSource()
	{
		return m_audioSource;
	}

	virtual std::string GetTypeString() { return "Audio"; }

private:
	AudioSource* m_audioSource = nullptr;
};