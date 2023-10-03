#pragma once

#include <memory>
#include "Asset.h"
#include "Audio/Audio.h"
#include "Core/Application.h"

class AudioAsset : public Asset
{
public:
	AudioAsset(const std::filesystem::path& path)
		: Asset(path, AssetType::Audio)
	{
		AudioClip* audioClip = Application::Get().GetAudio().LoadAudioClip(path);
		m_audioClip = std::unique_ptr<AudioClip>(audioClip);
		m_audioClip->Load();
	}

	AudioClip* GetAudioClip()
	{
		return m_audioClip.get();
	}

	virtual std::string GetTypeString() { return "Audio"; }

private:
	std::unique_ptr<AudioClip> m_audioClip = nullptr;
};