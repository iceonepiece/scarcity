#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include <filesystem>
#include "Game/GameApplication.h"
#include "BuildSerializer.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	BuildConfigs configs = Load(std::filesystem::current_path() / "build.json");

	GameApplication game({
		configs.name,
		configs.width,
		configs.height,
	});

	game.Initialize();
	game.OpenProject(configs.path);
	game.Run();

	//_CrtDumpMemoryLeaks();

	return 0;
}
