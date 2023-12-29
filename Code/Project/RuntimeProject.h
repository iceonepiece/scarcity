#include "Project.h"
#include "Platforms/OpenGL/OpenGLAssetManager.h"

class RuntimeProject : public Project
{
public:
	virtual void Initialize() override
	{
		m_assetManager = std::make_unique<OpenGLAssetManager>();
	}
};