#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include "Core/Game.h"
#include "Editor/EditorApplication.h"
#include "Core/GameApplication.h"

#include "Scenes/MenuScene.h"
#include "Scenes/Level1.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	/*
	EditorApplication editor;
	editor.Initialize();
	editor.Run();
	*/
	
	GameApplication game;

	game.AddScene("menu", new MenuScene(&game));
	game.AddScene("level1", new Level1(&game));

	game.Initialize("Scarcity", 1280, 720);
	game.ChangeScene("menu");

	game.Run();

	//_CrtDumpMemoryLeaks();

	return 0;
}
