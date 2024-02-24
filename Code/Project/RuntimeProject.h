#include "Project.h"
#include "Asset/AssetManager.h"

class RuntimeProject : public Project
{
public:
	RuntimeProject(const std::filesystem::path filepath)
		: Project(filepath)
	{}

	RuntimeProject(const std::string& name, const std::filesystem::path& path)
		: Project(name, path)
	{}

	virtual void Initialize() override
	{
		m_assetManager = std::make_unique<AssetManager>();
		m_assetManager->InitializeAssets(m_directory);
	}
};