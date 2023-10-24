#pragma once

#include "Components/UIComponents.h"
#include "Components/ButtonComponent.h"
#include "Input/NewInput.h"

class UIManager
{
public:
	static void ProcessButton(ButtonComponent& button, NewInput& input);
	static bool HandleInput(CanvasComponent& canvas, ButtonComponent& button, NewInput& input);
};