#pragma once

#include <string>
#include <memory>
#include <filesystem>
#include <unordered_map>
#include "AudioSource.h"
#include "AudioClip.h"

class Audio
{
public:
	static Audio* Get() { return s_instance; }
	static void Create();
	static void Shutdown();

	virtual void Initialize() = 0;
	virtual void Destroy() = 0;

	virtual AudioClip* LoadAudioClip(const std::filesystem::path& filePath) = 0;
	virtual bool LoadSound(const std::string& name, const std::string& filePath) = 0;
	virtual void UnloadSound(const std::string& name) = 0;

	virtual void PlaySound(AudioClip* audioClip) = 0;
	virtual void PlaySound(const std::string& name) = 0;
	virtual void PauseSound(const std::string& name) = 0;
	virtual void ResumeSound(const std::string& name) = 0;
	virtual void StopSound(const std::string& name) = 0;

	virtual void SetVolume(float volume) = 0;

	virtual AudioSource* CreateAudioSource() = 0;

	AudioSource* GetAudioSource(const std::string& name)
	{
		if (s_audioSources.find(name) != s_audioSources.end())
			return s_audioSources[name].get();

		return nullptr;
	}

protected:
	static std::unordered_map<std::string, std::unique_ptr<AudioSource>> s_audioSources;

private:
	static Audio* s_instance;
};
