#include "LuaEngine.h"
#include "Core/Application.h"

LuaEngine::LuaEngine()
{
	m_state.open_libraries(sol::lib::base);

	//m_state.set_function("LoadTexture", &ResourceAPI::LoadTexture);

	m_state.set_function("LoadTexture", [&](const std::string& name, const char* filename, bool alpha)
	{
		return Application::Get().GetAssetManager().LoadTexture(name, filename, alpha);
	});
}

void LuaEngine::ReadScript(const std::string& fileName)
{
	m_state.script_file(fileName);
}