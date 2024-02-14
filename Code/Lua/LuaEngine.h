#pragma once

#include <string>
#include <sol/sol.hpp>
#include "Core/Application.h"

class LuaEngine
{
public:
	LuaEngine();

	void ReadScript(const std::string& fileName);
	sol::function GetFunction(const std::string& name);
	sol::object GetValue(const std::string& name);
	sol::table GetTable(const std::string& name);
	void SetInt(const std::string& name, int value);

	sol::state& GetState()
	{
		return m_state;
	}

	Application& GetApplication()
	{
		return *m_app;
	}

private:
	sol::state m_state;
	Application* m_app = nullptr;
};