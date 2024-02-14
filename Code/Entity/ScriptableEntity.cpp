#include "ScriptableEntity.h"

void ScriptableEntity::Destroy()
{
	m_scene->OnDestroyEntity(m_entity.GetEntity());
}

void ScriptableEntity::RunLuaScript(const std::string& fileName, const std::string& functionName, sol::object data)
{
	LuaEngine& engine = m_scene->GetLuaEngine();
	engine.ReadScript(fileName);
	sol::function fn = engine.GetFunction(functionName);

	if (fn.valid())
		fn(data);
}