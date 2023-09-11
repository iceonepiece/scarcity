#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include "SandboxApplication.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//GameApplication game;
	SandboxApplication sandbox;
	sandbox.Initialize("Sandbox", 1280, 720);
	sandbox.Run();

	//game.AddScene("menu", new MenuScene(game));
	//game.AddScene("level1", new MyLevel());
	//game.AddScene("sample", new SampleScene());
	//game.LoadScenes();
	//game.ChangeScene("level1");

	//_CrtDumpMemoryLeaks();

	return 0;
}
