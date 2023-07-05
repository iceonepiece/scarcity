#pragma once

#include <string>
#include <angelscript/angelscript.h>
#include <iostream>

class ScriptingEngine
{
public:
	ScriptingEngine();
	~ScriptingEngine();

	void Execute(const std::string& filepath);

	static void Echo()
	{
		std::cout << "Echo !!!" << std::endl;
	}

private:
	asIScriptEngine* m_engine;
	asIScriptContext* m_context;
};