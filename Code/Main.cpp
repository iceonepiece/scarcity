#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include "Core/Game.h"
#include "Editor/EditorApplication.h"
#include "Core/GameApplication.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	/*
	Game game;
	game.Init();
	game.Run();
	*/

	EditorApplication editor;
	editor.Initialize();
	editor.Run();
	
	/*
	GameApplication game;
	game.Initialize();
	game.Run();
	*/

	//_CrtDumpMemoryLeaks();

	return 0;
}
