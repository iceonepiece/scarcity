#include "LuaEngine.h"
#include "Core/ResourceAPI.h"

LuaEngine::LuaEngine()
{
	m_state.open_libraries(sol::lib::base);
	m_state.set_function("LoadTexture", &ResourceAPI::LoadTexture);
}

void LuaEngine::ReadScript(const std::string& fileName)
{
	m_state.script_file(fileName);
}