#include "ScriptableEntity.h"
#include "Scene.h"

void ScriptableEntity::Destroy()
{
	m_scene->OnDestroyEntity(m_entity.GetEntity());
}