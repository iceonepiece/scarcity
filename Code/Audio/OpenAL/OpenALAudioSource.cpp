#include "OpenALAudioSource.h"
#include "OpenALAudioClip.h"

OpenALAudioSource::OpenALAudioSource()
{
    alGenSources(1, &m_source);
    alSourcef(m_source, AL_PITCH, 1);
    alSourcef(m_source, AL_GAIN, 1.0f);
    alSource3f(m_source, AL_POSITION, 0, 0, 0);
    alSource3f(m_source, AL_VELOCITY, 0, 0, 0);
    alSourcei(m_source, AL_LOOPING, AL_FALSE);
    //alSourcei(source, AL_BUFFER, buffer);

}

void OpenALAudioSource::Play()
{
	alSourcePlay(m_source);
}

void OpenALAudioSource::Play(AudioClip* audioClip)
{
    if (OpenALAudioClip* alClip = dynamic_cast<OpenALAudioClip*>(audioClip))
    {
        alSourcei(m_source, AL_BUFFER, alClip->m_buffer);
        alSourcePlay(m_source);
    }
}

void OpenALAudioSource::Destroy()
{
	alDeleteSources(1, &m_source);
}

void OpenALAudioSource::SetLooping(bool looping)
{
    alSourcei(m_source, AL_LOOPING, looping ? 1 : 0);
}