#include "LuaEngine.h"
#include "Core/Application.h"
#include "Project/Project.h"
#include "UI/UIManager.h"
#include "Graphics/Renderer.h"

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

	m_state.set_function("CreateUI", [&]()
	{
		glm::vec2 screenSize = Application::Get().GetRenderer().GetScreenSize();

		sol::optional<sol::table> uiTableExist = m_state["ui"];

		if (uiTableExist == sol::nullopt)
			return;

		sol::table uiTable = m_state["ui"];

		UIRect screenRect = { {0, 0}, screenSize, 0 };
		unsigned int index = 1;

		while (true)
		{
			sol::optional<sol::table> uiComponentExist = uiTable[index];

			if (uiComponentExist == sol::nullopt)
				return;

			sol::table uiComponent = uiTable[index];

			UIManager::CreateUIComponent(uiComponent, screenRect);
			index++;
		}

		std::cout << "UI Table exist\n";
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