#include "LuaEngine.h"
#include "Core/Application.h"
#include "Project/Project.h"

LuaEngine::LuaEngine()
{
	m_state.open_libraries(sol::lib::base);


	m_state.set_function("Get", [&](const std::string& name)
	{
		return Project::GetActive()->GetGlobalLuaEngine().GetValue(name);
	});

	m_state.set_function("GetKey", [&](const std::string& name)
	{
		return Application::Get().GetInput().GetKey(Key::Space);
	});
}

void LuaEngine::ReadScript(const std::string& fileName)
{
	m_state.script_file(fileName);
}

sol::function LuaEngine::GetFunction(const std::string& name)
{
	return m_state[name];
}

sol::object LuaEngine::GetValue(const std::string& name)
{
	return m_state["gameData"][name];
}