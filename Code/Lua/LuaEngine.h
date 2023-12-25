#pragma once

#include <string>
#include <sol/sol.hpp>

class LuaEngine
{
public:
	LuaEngine();

	void ReadScript(const std::string& fileName);
	sol::function GetFunction(const std::string& name);

private:
	sol::state m_state;
};