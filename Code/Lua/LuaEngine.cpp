#include "LuaEngine.h"
#include "Core/Application.h"
#include "Project/Project.h"
#include "Graphics/Renderer.h"
#include "LuaUI.h"
#include "Game/GameLayer.h"

std::unordered_map<std::string, KeyCode> keyMap{
	{"left", Key::Left },
	{"right", Key::Right },
	{"up", Key::Up },
	{"down", Key::Down },
	{"escape", Key::Escape }
};

LuaEngine::LuaEngine()
{
	m_app = &Application::Get();
	m_state.open_libraries(sol::lib::base);

	m_state.set_function("DB_SetInt", [&](const std::string& name, int value)
	{
		return Project::GetActive()->SetNativeInt(name, value);
	});

	m_state.set_function("Get", [&](const std::string& name)
	{
		return Project::GetActive()->GetGlobalLuaEngine().GetValue(name);
	});

	m_state.set_function("GetTable", [&](const std::string& name)
	{
		return Project::GetActive()->GetGlobalLuaEngine().GetTable(name);
	});

	m_state.set_function("SetInt", [&](const std::string& name, int value)
	{
		return Project::GetActive()->GetGlobalLuaEngine().SetInt(name, value);
	});

	m_state.set_function("GetKey", [&](const std::string& name)
	{
		return Application::Get().GetInput().GetKey(Key::Space);
	});

	m_state.set_function("GetKeyDown", [&](const std::string& name)
	{
		if (keyMap.find(name) != keyMap.end())
		{
			return Application::Get().GetInput().GetKeyDown(keyMap[name]);
		}

		return false;
	});

	m_state.set_function("ChangeScene", [&](const std::string& name)
	{
		Application::Get().ChangeScene(name);
	});

	m_state.set_function("Shutdown", [&]()
	{
		m_app->StopGame();
	});

	BindLuaUI(*this);
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

sol::table LuaEngine::GetTable(const std::string& name)
{
	return m_state["gameData"][name];
}

void LuaEngine::SetInt(const std::string& name, int value)
{
	m_state["gameData"][name] = value;
}