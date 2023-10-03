#pragma once

#include "AudioClip.h"

class AudioSource
{
public:
	virtual void Play() = 0;
	virtual void Destroy() = 0;
	virtual void Play(AudioClip* audioClip) = 0;
};