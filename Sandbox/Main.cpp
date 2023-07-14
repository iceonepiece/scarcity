#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include "Core/GameApplication.h"
#include "SandboxApplication.h"
#include "Stages/MenuScene.h"
#include "Stages/MyLevel.h"
#include "Stages/SampleScene.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//GameApplication game;
	GameApplication game;
	game.Initialize("Sandbox", 1280, 720);

	//game.AddScene("menu", new MenuScene(game));
	game.AddScene("level1", new MyLevel());
	//game.AddScene("sample", new SampleScene());
	game.LoadScenes();
	game.ChangeScene("level1");

	game.Run();

	//_CrtDumpMemoryLeaks();

	return 0;
}
