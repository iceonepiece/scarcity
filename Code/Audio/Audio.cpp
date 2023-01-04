#include "Audio.h"
#include "Wav.h"
#include <iostream>
#include <vector>

ALCdevice* Audio::s_device;
ALCcontext* Audio::s_context;
std::unordered_map<std::string, AudioSource> Audio::s_audioSources;

void Audio::Init()
{
	s_device = alcOpenDevice(NULL);
	if (!s_device)
	{
		std::cerr << "ERROR: Could not open audio device" << std::endl;
	}

    s_context = alcCreateContext(s_device, NULL);
    if (!s_context)
    {
        std::cerr << "ERROR: Could not create audio context" << std::endl;
    }

	if (!alcMakeContextCurrent(s_context))
	{
        std::cerr << "ERROR: Could not make audio context current" << std::endl;
	}
}

void Audio::PlaySound(std::string name)
{
    if (s_audioSources.find(name) == s_audioSources.end())
        return;
   
    AudioSource audioSource = s_audioSources[name];

    alSourcePlay(audioSource.source);

    /*
    ALint state = AL_PLAYING;

    while (state == AL_PLAYING)
    {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    }
    */
}


bool Audio::LoadSound(std::string name, std::string filePath)
{
    std::uint8_t channels;
    std::int32_t sampleRate;
    std::uint8_t bitsPerSample;
    std::vector<char> soundData;

    if (!load_wav(filePath, channels, sampleRate, bitsPerSample, soundData))
    {
        std::cerr << "ERROR: Could not load wav" << std::endl;
        return false;
    }

    ALuint buffer;
    alGenBuffers(1, &buffer);

    ALenum format;
    if (channels == 1 && bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if (channels == 1 && bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if (channels == 2 && bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if (channels == 2 && bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;
    else
    {
        std::cerr
            << "ERROR: unrecognised wave format: "
            << channels << " channels, "
            << bitsPerSample << " bps" << std::endl;
    }

    alBufferData(buffer, format, soundData.data(), soundData.size(), sampleRate);
    soundData.clear(); // erase the sound in RAM

    ALuint source;
    alGenSources(1, &source);
    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, 1.0f);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcei(source, AL_BUFFER, buffer);

    s_audioSources.emplace(name, AudioSource { buffer, source });
}

void Audio::Destroy()
{
    for (auto& [_name, audioSource] : s_audioSources)
    {
        alDeleteSources(1, &(audioSource.source));
        alDeleteBuffers(1, &(audioSource.buffer));
    }
    s_audioSources.clear();

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(s_context);
    alcCloseDevice(s_device);
}