#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include "EditorApplication.h"
// #include "Editor2D.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ApplicationConfigs configs;
	configs.name = "BossFight Engine";

	EditorApplication editor(configs);
	editor.Initialize("BossFight Engine", 1280, 720);
	editor.Run();

	//_CrtDumpMemoryLeaks();

	return 0;
}
