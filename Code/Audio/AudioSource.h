#pragma once

#include "AudioClip.h"

class AudioSource
{
public:
	virtual void Play() = 0;
	virtual void Destroy() = 0;
	virtual void Play(AudioClip* audioClip) = 0;
	virtual void SetLooping(bool looping) = 0;
};

class NullAudioSource : public AudioSource
{
public:
	virtual void Play() {}
	virtual void Destroy() {}
	virtual void Play(AudioClip* audioClip) {}
	virtual void SetLooping(bool looping) {}
};