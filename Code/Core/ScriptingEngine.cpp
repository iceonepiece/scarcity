#include "ScriptingEngine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <angelscript/scriptstdstring/scriptstdstring.h>

void print(std::string& msg)
{
	std::cout << msg;
}

void printNumber(int number)
{
	std::cout << number;
}

namespace AS
{
	class Circle
	{
	public:

		void Addref()
		{
			// Increase the reference counter
			refCount++;
		}

		void Release()
		{
			// Decrease ref count and delete if it reaches 0
			if (--refCount == 0)
				delete this;
		}

		Circle()
		{
			refCount = 1;
			m_area = 51;
		}

		int GetArea()
		{
			return m_area;
		}

		void SetArea(int n)
		{
			m_area = n;
		}

	private:
		int refCount = 0;

		int m_area;

	};

	Circle* Ref_Factory()
	{
		return new Circle();
	}


	std::string loadFileToString(const std::string& filename) {
		std::ifstream file(filename);
		if (!file.is_open()) {
			// File failed to open, handle the error
			std::cout << "Failed to open file: " + filename << std::endl;
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf(); // Read the entire file into the buffer
		file.close();

		// Return the contents of the buffer as a string
		return buffer.str();
	}

	void MessageCallback(const asSMessageInfo* msg, void* param)
	{
		const char* type = "ERR ";
		if (msg->type == asMSGTYPE_WARNING)
			type = "WARN";
		else if (msg->type == asMSGTYPE_INFORMATION)
			type = "INFO";

		printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
	}

}

ScriptingEngine::ScriptingEngine()
{
	std::cout << "ScriptingEngine Created!!!" << std::endl;

	m_engine = asCreateScriptEngine();
	if (m_engine == 0)
	{
		std::cout << "Failed to create script engine." << std::endl;
		return;
	}

	m_engine->SetMessageCallback(asFUNCTION(AS::MessageCallback), 0, asCALL_CDECL);

	int r;

	RegisterStdString(m_engine);

	r = m_engine->RegisterGlobalFunction("void print(string &in)", asFUNCTION(print), asCALL_CDECL);
	r = m_engine->RegisterGlobalFunction("void printNumber(int)", asFUNCTION(printNumber), asCALL_CDECL);

	m_engine->RegisterObjectType("Circle", 0, asOBJ_REF); // asOBJ_REF because you wanted a reference call
	m_engine->RegisterObjectBehaviour("Circle", asBEHAVE_FACTORY, "Circle@ f()", asFUNCTION(AS::Ref_Factory), asCALL_CDECL);
	m_engine->RegisterObjectBehaviour("Circle", asBEHAVE_ADDREF, "void f()", asMETHOD(AS::Circle, Addref), asCALL_THISCALL);
	m_engine->RegisterObjectBehaviour("Circle", asBEHAVE_RELEASE, "void f()", asMETHOD(AS::Circle, Release), asCALL_THISCALL);
	m_engine->RegisterObjectMethod("Circle", "int GetArea()", asMETHOD(AS::Circle, GetArea), asCALL_THISCALL);
	m_engine->RegisterObjectMethod("Circle", "void SetArea(int)", asMETHOD(AS::Circle, SetArea), asCALL_THISCALL);

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

void ScriptingEngine::Execute(const std::string& filepath)
{
	int r;

	std::string script = AS::loadFileToString(filepath);

	asIScriptModule* mod = m_engine->GetModule(0, asGM_ALWAYS_CREATE);
	r = mod->AddScriptSection("script", &script[0], script.size());

	if (r < 0)
	{
		std::cout << "AddScriptSection() failed" << std::endl;
	}

	r = mod->Build();
	if (r < 0)
	{
		std::cout << "Build() failed" << std::endl;
	}

	asIScriptFunction* func = m_engine->GetModule(0)->GetFunctionByDecl("void main()");

	if (func == 0)
	{
		std::cout << "The function 'void main()' was not found." << std::endl;
	}

	m_context->Prepare(func);
	m_context->Execute();
}