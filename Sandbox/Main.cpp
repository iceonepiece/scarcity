#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include "Core/GameApplication.h"
#include "Stages/MenuScene.h"
#include "Stages/MyLevel.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GameApplication game;
	game.Initialize("Scarcity", 1280, 720);

	game.AddScene("menu", new MenuScene(game));
	game.AddScene("level1", new MyLevel());
	game.LoadScenes();
	game.ChangeScene("level1");

	game.Run();

	//_CrtDumpMemoryLeaks();

	return 0;
}
