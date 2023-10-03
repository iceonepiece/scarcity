#pragma once

class AudioSource
{
public:
	virtual void Play() = 0;
	virtual void Destroy() = 0;

	inline float GetDuration()
	{
		return m_duration;
	}

protected:
	float m_duration;

};