#pragma once

#include <string>
#include <sol/sol.hpp>

class LuaEngine
{
public:
	LuaEngine();

	void ReadScript(const std::string& fileName);

private:
	sol::state m_state;
};