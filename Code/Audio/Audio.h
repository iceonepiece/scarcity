#include <string>

class Audio
{
public:
	virtual void Initialize() = 0;
	virtual void Destroy() = 0;

	virtual bool LoadSound(const std::string& name, const std::string& filePath) = 0;
	virtual void UnloadSound(const std::string& name) = 0;

	virtual void PlaySound(const std::string& name) = 0;
	virtual void PauseSound(const std::string& name) = 0;
	virtual void ResumeSound(const std::string& name) = 0;
	virtual void StopSound(const std::string& name) = 0;

	virtual void SetVolume(float volume) = 0;
};