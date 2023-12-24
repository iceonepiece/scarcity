#include "Audio.h"
#include "OpenAL/OpenALAudio.h"

Audio* Audio::s_instance = nullptr;
std::unordered_map<std::string, std::unique_ptr<AudioSource>> Audio::s_audioSources;

void Audio::Create()
{
	s_instance = new OpenALAudio();
}

void Audio::Shutdown()
{
	delete s_instance;
}