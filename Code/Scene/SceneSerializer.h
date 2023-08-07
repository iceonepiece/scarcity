#include <string>
#include <filesystem>
#include "Scene.h"

class SceneSerializer
{
public:
	static bool Serialize(Scene& scene, std::filesystem::path filePath);
	static bool Deserialize(Scene& scene, std::filesystem::path filePath);
};