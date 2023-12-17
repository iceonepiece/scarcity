#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include "Game/GameApplication.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ApplicationConfigs configs;
	configs.name = "Bossfight Sandbox";
	configs.width = 1280;
	configs.height = 720;

	GameApplication game(configs);
	game.Initialize();
	game.OpenProject("C:\\study\\year2_1\\Advanced Project 1\\bossfight-demo\\BossFight Demo.bfproj");
	game.Run();

	//game.AddScene("menu", new MenuScene(game));
	//game.AddScene("level1", new MyLevel());
	//game.AddScene("sample", new SampleScene());
	//game.LoadScenes();
	//game.ChangeScene("level1");

	//_CrtDumpMemoryLeaks();

	return 0;
}
