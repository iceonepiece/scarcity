#pragma once

#include "Components/UIComponents.h"
#include "Components/ButtonComponent.h"
#include "Input/Input.h"
#include "UIComponent.h"
#include <sol/sol.hpp>

class UIManager
{
public:
	static void ProcessButton(ButtonComponent& button, Input& input);
	static bool HandleInput(CanvasComponent& canvas, ButtonComponent& button, Input& input);
	static void CreateUIComponent(sol::table& uiComponent, const UIRect& parentRect = {});

	static void AddObject(const UIObject& object)
	{
		m_objects.push_back(object);
	}

	static std::vector<UIObject> m_objects;
};