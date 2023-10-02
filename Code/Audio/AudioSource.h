#pragma once

class AudioSource
{
public:
	virtual void Play() = 0;
	virtual void Destroy() = 0;
};