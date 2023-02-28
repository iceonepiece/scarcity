#pragma once

#include <sol/sol.hpp>
#include "../Core/Scene.h"

class LuaScene : public Scene
{
public:
	LuaScene(Application* game)
		: Scene(game)
	{

	}

	virtual void Init() override
	{
		lua.open_libraries(sol::lib::base, sol::lib::package);
		lua.script("print('bark bark bark!')");
	}
	
	virtual void Update(float deltaTime) override
	{
	}

protected:
	sol::state lua;
};