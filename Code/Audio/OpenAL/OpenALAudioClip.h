#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include "Audio/AudioClip.h"
#include "Audio/Wav.h"

class OpenALAudioClip : public AudioClip
{
public:
	OpenALAudioClip(const std::filesystem::path& filePath)
		: AudioClip(filePath)
	{
	}

	virtual bool Load() override
	{
        std::uint8_t channels;
        std::int32_t sampleRate;
        std::uint8_t bitsPerSample;
        std::vector<char> soundData;

        if (!load_wav(m_filePath.string(), channels, sampleRate, bitsPerSample, soundData))
        {
            std::cerr << "ERROR: Could not load wav" << std::endl;
            return false;
        }

        alGenBuffers(1, &m_buffer);

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

        alBufferData(m_buffer, format, soundData.data(), soundData.size(), sampleRate);
        soundData.clear(); // erase the sound in RAM

        UpdateDuration();

        return true;
	}

    void UpdateDuration()
    {
        ALint sizeInBytes;
        ALint channels;
        ALint bits;

        alGetBufferi(m_buffer, AL_SIZE, &sizeInBytes);
        alGetBufferi(m_buffer, AL_CHANNELS, &channels);
        alGetBufferi(m_buffer, AL_BITS, &bits);

        int lengthInSamples = sizeInBytes * 8 / (channels * bits);

        ALint frequency;
        alGetBufferi(m_buffer, AL_FREQUENCY, &frequency);

        m_duration = (float)lengthInSamples / (float)frequency;
    }

	virtual ~OpenALAudioClip() override
	{
		alDeleteBuffers(1, &m_buffer);
	}

private:
	ALuint m_buffer;

    friend class OpenALAudioSource;
};