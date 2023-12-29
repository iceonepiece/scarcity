#pragma once

#include <memory>
#include <filesystem>
#include "Core/TagManager.h"
#include "Asset/AssetManager.h"
#include "Entity/EntityManager.h"
#include "Scene/Scene.h"

class Project
{
public:
	Project()
	{
		s_activeProject = this;
	}

	Project(const std::string& name, const std::filesystem::path& path)
		: m_name(name)
		, m_absolutePath(path)
		, m_directory(path)
	{
		s_activeProject = this;
	}

	virtual ~Project() = default;

	virtual void Initialize() = 0;

	bool Save();
	std::unique_ptr<Scene> LoadScene(const std::filesystem::path& relativePath);

	inline void SetStartScene(std::filesystem::path path) { m_startScene = path; }
	inline std::filesystem::path GetStartScene() { return m_startScene; }
	inline std::filesystem::path GetStartSceneAbsolutePath() { return m_absolutePath / m_startScene;  }

	inline std::string GetName() { return m_name; }
	inline std::filesystem::path GetDirectory() { return m_directory; }
	inline std::filesystem::path GetAbsolutePath() { return m_directory; }

	inline TagManager& GetTagManager() { return m_tagManager; }

	static bool SaveActive();
	static Project* GetActive() { return s_activeProject; }

	void AddPrefab(Entity entity);
	Entity GetPrefabByName(const std::string& name);

	inline AssetManager& GetAssetManager() { return *m_assetManager; }
	inline EntityManager& GetPrefabManager() { return m_prefabManager; }

protected:
	std::string m_name = "Untitled";
	std::filesystem::path m_directory;
	std::filesystem::path m_absolutePath;
	std::filesystem::path m_startScene;

	std::unique_ptr<AssetManager> m_assetManager;
	EntityManager m_prefabManager;
	std::unordered_map<std::string, Entity> m_prefabMap;

	TagManager m_tagManager;

	inline static Project* s_activeProject;

	friend class ProjectSerializer;
};