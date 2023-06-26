#include "ScriptingEngine.h"
#include <iostream>

ScriptingEngine::ScriptingEngine()
{
	std::cout << "ScriptingEngine Created!!!" << std::endl;

	m_engine = asCreateScriptEngine();
	if (m_engine == 0)
	{
		std::cout << "Failed to create script engine." << std::endl;
		return;
	}

	m_context = m_engine->CreateContext();
	if (m_context == 0)
	{
		std::cout << "Failed to create the context." << std::endl;
		m_engine->Release();
		return;
	}
}

ScriptingEngine::~ScriptingEngine()
{
	// We must release the contexts when no longer using them
	m_context->Release();

	// Shut down the engine
	m_engine->ShutDownAndRelease();
}