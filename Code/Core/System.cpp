#include "System.h"
#include "Scene/Scene.h"

System::System(Scene* scene)
	: m_scene(scene)
	, m_registry(scene->GetEntityManager().m_registry)
{
}

