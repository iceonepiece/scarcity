#include "LuaEngine.h"
#include "Core/Application.h"
#include "Project/Project.h"

LuaEngine::LuaEngine()
{
	m_state.open_libraries(sol::lib::base);

	m_state.set_function("LoadTexture", [&](const std::string& name, const char* filename, bool alpha)
	{
		return Project::GetActive()->GetAssetManager().LoadTexture(name, filename, alpha);
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