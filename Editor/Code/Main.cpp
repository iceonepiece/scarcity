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
	configs.width = 1600;
	configs.height = 900;

	EditorApplication editor(configs);
	editor.Initialize();
	editor.Run();

	//_CrtDumpMemoryLeaks();

	return 0;
}
