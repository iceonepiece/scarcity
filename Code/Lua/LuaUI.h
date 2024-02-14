#pragma once

#include <sol/sol.hpp>
#include "UI/UIManager.h"

void BindLuaUI(LuaEngine& engine)
{
	sol::state& m_state = engine.GetState();

	UIManager& uiManager = engine.GetApplication().GetUIManager();

	m_state.set_function("UI_SetFontColor", [&](float r, float g, float b, float a)
	{
		uiManager.s_fontColor = glm::vec4(r, g, b, a);
	});

	m_state.set_function("UI_SetFontSize", [&](float size)
	{
		uiManager.s_fontSize = size;
	});

	m_state.set_function("UI_SetBackgroundColor", [&](float r, float g, float b, float a)
	{
		uiManager.s_backgroundColor = glm::vec4(r, g, b, a);
	});

	m_state.set_function("UI_Panel", [&](float x, float y, float width, float height)
	{
		UIObject* uiObject = new UIButtonObject();
		uiObject->type = UIType::UIType_Box;
		uiObject->position = glm::vec3{ x, y, 0.0f };
		uiObject->scale = glm::vec3{ width, height, 1 };
		uiObject->color = uiManager.s_backgroundColor;
		uiObject->fontSize = uiManager.s_fontSize;
		uiObject->fontColor = uiManager.s_fontColor;
		uiManager.AddObject(uiObject);
	});

	m_state.set_function("UI_Button", [&](float x, float y, float width, float height, std::string text, sol::function callback)
	{
		UIObject* uiObject = new UIButtonObject();
		uiObject->type = UIType::UIType_Button;
		uiObject->position = glm::vec3{ x, y, 0.0f };
		uiObject->scale = glm::vec3{ width, height, 1 };
		uiObject->color = uiManager.s_backgroundColor;
		uiObject->fontSize = uiManager.s_fontSize;
		uiObject->fontColor = uiManager.s_fontColor;
		uiObject->text = text;
		uiObject->onClickFunction = callback;
		uiManager.AddObject(uiObject);
	});

	m_state.set_function("UI_Slider", [&](float x, float y, float width, float height, int value)
	{
		UIObject* uiObject = new UIObject();
		uiObject->type = UIType::UIType_Box;
		uiObject->position = glm::vec3{ x, y, 0.0f };
		uiObject->scale = glm::vec3{ width, height, 1.0f };
		uiManager.AddObject(uiObject);

		UIObject* uiText = new UIObject();
		uiText->type = UIType::UIType_Text;
		uiText->position = glm::vec3{ x, y, 0.0f };
		uiText->fontSize = uiManager.s_fontSize;
		uiText->fontColor = uiManager.s_fontColor;
		uiText->text = std::to_string(value);
		uiManager.AddObject(uiText);
	});

	m_state.set_function("UI_Text", [&](std::string text, float x, float y, float size)
	{
		UIObject* uiObject = new UIObject();
		uiObject->type = UIType::UIType_Text;
		uiObject->position = glm::vec3{ x, y, 0.0f };
		uiObject->fontSize = size;
		uiObject->fontColor = uiManager.s_fontColor;
		uiObject->text = text;

		uiManager.AddObject(uiObject);
	});

	m_state.set_function("UI_Select", [&](float x, float y, sol::object keys, int selected, bool active)
	{
		if (keys.is<sol::table>())
		{
			sol::table keyTable = keys;

			unsigned int index = 1;

			float offsetX = x;
			float fontSpacing = 0.0f;
			float fontWidth = uiManager.s_fontSize * 0.6f;

			while (true)
			{
				sol::optional<sol::object> exist = keyTable[index];

				if (exist == sol::nullopt)
					return;

				sol::object key = keyTable[index];
				std::string text = key.as<std::string>();

				UIObject* uiObject = new UIObject();
				uiObject->type = UIType::UIType_Text;
				uiObject->position = glm::vec3{ offsetX, y, 0.0f};
				uiObject->fontSize = uiManager.s_fontSize;
				uiObject->fontColor = uiManager.s_fontColor;

				if (selected + 1 == index)
				{
					uiObject->fontColor.x *= 1.5f;
					uiObject->fontColor.y *= 1.5f;
					uiObject->fontColor.z *= 1.5f;
				}

				uiObject->text = text;
				uiManager.AddObject(uiObject);

				offsetX += (fontWidth + fontSpacing) * text.size();
				offsetX += 32.0f;
				index++;
			}
		}
	});

	m_state.set_function("CreateUI", [&]()
	{
		glm::vec2 screenSize = Application::Get().GetRenderer().GetScreenSize();

		sol::optional<sol::table> uiTableExist = m_state["ui"];

		if (uiTableExist == sol::nullopt)
			return;

		sol::table uiTable = m_state["ui"];

		UIRect screenRect = { {0, 0}, screenSize, 0 };
		unsigned int index = 1;

		while (true)
		{
			sol::optional<sol::table> uiComponentExist = uiTable[index];

			if (uiComponentExist == sol::nullopt)
				return;

			sol::table uiComponent = uiTable[index];

			uiManager.CreateUIComponent(uiComponent, screenRect);
			index++;
		}

		std::cout << "UI Table exist\n";
	});
}