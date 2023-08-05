#include <string>
#include <filesystem>
#include "Scene.h"

class SceneSerializer
{
public:
	SceneSerializer(Scene& scene);

	void Serialize(std::filesystem::path filepath);
	bool Deserialize(const std::string& filepath);

private:
	Scene& m_scene;
};