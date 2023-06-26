#pragma once

#include <angelscript/angelscript.h>

class ScriptingEngine
{
public:
	ScriptingEngine();
	~ScriptingEngine();

private:
	asIScriptEngine* m_engine;
	asIScriptContext* m_context;
};