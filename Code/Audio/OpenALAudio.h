#include <string>
#include <unordered_map>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include "Audio.h"

struct AudioSource
{
	ALuint buffer;
	ALuint source;
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
	static std::unordered_map<std::string, AudioSource> s_audioSources;
};