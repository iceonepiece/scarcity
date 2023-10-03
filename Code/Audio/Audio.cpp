#include "Audio.h"
#include "OpenAL/OpenALAudio.h"

std::unordered_map<std::string, std::unique_ptr<AudioSource>> Audio::s_audioSources;

std::unique_ptr<Audio> Audio::Create()
{
	return std::make_unique<OpenALAudio>();
}