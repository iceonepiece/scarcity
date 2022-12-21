#pragma once

#include <sol/sol.hpp>

class ScriptManager
{
public:

	void ReadScript(std::string fileName)
	{
		m_luaState.script_file(fileName);

        sol::table particles = m_luaState["particles"];
      

	}

private:
	sol::state m_luaState;
};