#include <string>
#include <filesystem>
#include "Scene.h"

class SceneSerializer
{
public:
	SceneSerializer(Scene& scene);

	static bool Serialize(Scene& scene, std::filesystem::path filePath);

	bool Serialize(std::filesystem::path filepath);
	bool Deserialize(std::filesystem::path filepath);

private:
	Scene& m_scene;
};