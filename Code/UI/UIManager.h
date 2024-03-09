#pragma once

#include "Components/UIComponents.h"
#include "Components/ButtonComponent.h"
#include "Input/Input.h"
#include "UI/UIComponent.h"
#include <sol/sol.hpp>
#include <vector>

class UIManager
{
public:
	void ProcessButton(ButtonComponent& button, Input& input);
	bool HandleInput(CanvasComponent& canvas, ButtonComponent& button, Input& input);
	void CreateUIComponent(sol::table& uiComponent, const UIRect& parentRect = {});

	void Clear();

	void AddObject(UIObject* object)
	{
		m_objects.emplace_back(object);
	}

	std::vector<std::unique_ptr<UIObject>> m_objects;

	glm::vec4 s_backgroundColor = glm::vec4(1.0f);
	glm::vec4 s_fontColor = glm::vec4(0.0f);
	bool s_active = true;
	float s_fontSize = 32.0f;
	std::string m_fontName = "";
	std::vector<UIRect> m_rectStack;
};