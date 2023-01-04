#include <string>
#include <unordered_map>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

struct AudioSource
{
	ALuint buffer;
	ALuint source;
};

class Audio
{
public:
	static void Init();
	static void Destroy();
	static bool LoadSound(std::string name, std::string filePath);
	static void PlaySound(std::string name);

private:
	static ALCdevice* s_device;
	static ALCcontext* s_context;
	static std::unordered_map<std::string, AudioSource> s_audioSources;
};