#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include "Audio/Audio.h"
#include "OpenALAudioSource.h"

class OpenALAudio : public Audio
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	
	virtual AudioSource* CreateAudioSource() override;
	virtual AudioClip* LoadAudioClip(const std::filesystem::path& filePath) override;

	virtual bool LoadSound(const std::string& name, const std::string& filePath) override;
	virtual void UnloadSound(const std::string& name) override {}

	virtual void PlaySound(AudioClip* audioClip) override;
	virtual void PlaySound(const std::string& name) override;
	virtual void PauseSound(const std::string& name) override {}

	virtual void ResumeSound(const std::string& name) override {}
	virtual void StopSound(const std::string& name) override {}

	virtual void SetVolume(float volume) override {}

private:
	static std::unique_ptr<OpenALAudioSource> s_mainAudioSource;
	static ALCdevice* s_device;
	static ALCcontext* s_context;
};