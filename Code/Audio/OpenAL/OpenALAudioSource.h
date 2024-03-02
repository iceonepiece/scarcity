#pragma once

#include <AL/al.h>
#include "Audio/AudioSource.h"

class OpenALAudioSource : public AudioSource
{
public:
	OpenALAudioSource();
	virtual void Play() override;
	virtual void Destroy() override;
	virtual void Play(AudioClip* audioClip) override;
	virtual void SetLooping(bool looping) override;
	virtual void SetVolume(float volume) override;

private:
	ALuint m_source;
};