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

	static void Clear();

	static void AddObject(UIObject* object)
	{
		m_objects.emplace_back(object);
	}

	static std::vector<std::unique_ptr<UIObject>> m_objects;

	inline static glm::vec4 s_backgroundColor = glm::vec4(1.0f);
	inline static glm::vec4 s_fontColor = glm::vec4(0.0f);
	inline static float s_fontSize = 32.0f;
};