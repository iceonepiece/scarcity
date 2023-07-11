#pragma once

#include <memory>
#include "../Core/Scriptable.h"

struct ScriptableComponent
{
	ScriptableComponent(Scriptable* s)
		: scriptable(s)
	{}

	std::unique_ptr<Scriptable> scriptable;
};