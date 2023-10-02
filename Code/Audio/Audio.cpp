#include "Audio.h"

std::unordered_map<std::string, std::unique_ptr<AudioSource>> Audio::s_audioSources;