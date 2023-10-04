#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "Audio/AudioSource.h"

using json = nlohmann::json;

struct AudioSourceComponent
{
	static std::string Name() { return "AudioSource"; }

	float volume = 1.0f;
	std::string audioClipPath = "";
	AudioSource* audioSource = nullptr;

	void Play(AudioClip* clip)
	{
		audioSource->Play(clip);
	}
};

static void DoSerialize(const AudioSourceComponent& audioSource, json& entityJson)
{
	entityJson[AudioSourceComponent::Name()]["volume"] = audioSource.volume;
	entityJson[AudioSourceComponent::Name()]["audioClipPath"] = audioSource.audioClipPath;
}

static void DoDeserialize(AudioSourceComponent& audioSource, json& audioJson)
{
	audioSource.volume = audioJson["volume"].get<float>();
	audioSource.audioClipPath = audioJson["audioClipPath"].get<std::string>();
}