#pragma once

#include "Components/UIComponents.h"
#include "Components/ButtonComponent.h"
#include "Input/Input.h"

class UIManager
{
public:
	static void ProcessButton(ButtonComponent& button, Input& input);
	static bool HandleInput(CanvasComponent& canvas, ButtonComponent& button, Input& input);
};