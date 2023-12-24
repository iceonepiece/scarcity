#include "ScriptableEntity.h"

void ScriptableEntity::Destroy()
{
	m_scene->OnDestroyEntity(m_entity.GetEntity());
}