#include <string>
#include <unordered_map>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include "Audio.h"
#include "AudioSource.h"

class OpenALAudioSource : public AudioSource
{
public:
	OpenALAudioSource(ALuint buffer, ALuint source)
		: m_buffer(buffer)
		, m_source(source)
	{

	}

	virtual void Play() override
	{
		alSourcePlay(m_source);
	}

	virtual void Destroy() override
	{
		alDeleteSources(1, &m_source);
		alDeleteBuffers(1, &m_buffer);
	}

private:
	ALuint m_buffer;
	ALuint m_source;
};

class OpenALAudio : public Audio
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	
	virtual bool LoadSound(const std::string& name, const std::string& filePath) override;
	virtual void UnloadSound(const std::string& name) override {}

	virtual void PlaySound(const std::string& name) override;
	virtual void PauseSound(const std::string& name) override {}


	virtual void ResumeSound(const std::string& name) override {}
	virtual void StopSound(const std::string& name) override {}

	virtual void SetVolume(float volume) override {}

private:
	static ALCdevice* s_device;
	static ALCcontext* s_context;
};