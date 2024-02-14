#include "Project.h"
#include "ProjectSerializer.h"
#include "Scene/Scene.h"
#include "Scene/SceneSerializer.h"
#include "File/FileSystem.h"
#include "Constants/GameEngine.h"

void Project::StartRunning()
{
	std::filesystem::path globalLuaPath = m_absolutePath / (m_name + ".lua");

	if (FileSystem::FileExists(globalLuaPath))
		m_globalLuaEngine.ReadScript(globalLuaPath.string());


	/*
	for (auto& luaScript : m_luaScripts)
	{
		if (m_luaEngineMap.find(luaScript->GetPath().string()) == m_luaEngineMap.end())
		{
			m_luaEngineMap[luaScript->GetPath().string()] = {};
			m_luaEngineMap[luaScript->GetPath().string()].ReadScript(luaScript->GetPath().string());
		}
	}
	*/
}

void Project::StopRunning()
{
	m_luaEngineMap.clear();
}

void Project::AddPrefab(Entity entity)
{
	m_prefabMap.insert({ entity.GetName(), entity });
}

Entity Project::GetPrefabByName(const std::string& name)
{
	if (m_prefabMap.find(name) != m_prefabMap.end())
		return m_prefabMap[name];

	return Entity{};
}

bool Project::Save()
{
	if (!std::filesystem::is_directory(m_directory / "ProjectSettings"))
		FileSystem::CreateFolder(m_directory / "ProjectSettings");

	std::filesystem::path projFilePath = m_directory / (m_name + PROJECT_FILE_EXTENSION);

	return ProjectSerializer::Serialize(*this, projFilePath);
}

std::unique_ptr<Scene> Project::LoadScene(const std::filesystem::path& relativePath)
{
	std::unique_ptr<Scene> scene = std::make_unique<Scene>();
	scene->SetProject(this);

	if (SceneSerializer::Deserialize(*scene, relativePath))
		return scene;

	return nullptr;
}

bool Project::SaveActive()
{
	return true;
}