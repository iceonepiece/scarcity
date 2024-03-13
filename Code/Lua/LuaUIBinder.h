#pragma once

#include <sol/sol.hpp>
#include "UI/UIManager.h"
#include "LuaEngine.h"
#include "Graphics/Renderer.h"
#include "Project/Project.h"

void BindLuaUI(LuaEngine& engine)
{
	sol::state& m_state = engine.GetState();

	UIManager& uiManager = engine.GetApplication().GetUIManager();

	m_state.set_function("UI_SetActive", [&](bool active)
	{
		uiManager.SetActive(active);
	});

	m_state.set_function("UI_Begin", [&](float x, float y, float width, float height, int flags)
	{
		glm::vec2 absolutePosition = uiManager.GetAbsolutePosition(width, height, (UIFlag)flags);

		absolutePosition.x += x;
		absolutePosition.y += y;

		UIObject* uiObject = new UIObject(
			{ absolutePosition.x, absolutePosition.y, 0 },
			{ width, height, 1 }
		);
		uiObject->flags = (UIFlag)flags;
		uiObject->backgroundColor = uiManager.GetBackgroundColor();
		uiObject->fontSize = uiManager.GetFontSize();
		uiObject->fontColor = uiManager.GetFontColor();
		uiManager.AddObject(uiObject);

		uiManager.PushRect({ absolutePosition, {width, height}, 0 });
	});

	m_state.set_function("UI_End", [&]()
	{
		uiManager.PopRect();
	});

	m_state.set_function("UI_SetFontColor", [&](float r, float g, float b, float a)
	{
		uiManager.SetFontColor({ r, g, b, a });
	});

	m_state.set_function("UI_SetFontSize", [&](float size)
	{
		uiManager.SetFontSize(size);
	});

	m_state.set_function("UI_SetFontName", [&](const std::string& name)
	{
		uiManager.SetFontName(name);
	});

	m_state.set_function("UI_SetBackgroundColor", [&](float r, float g, float b, float a)
	{
		uiManager.SetBackgroundColor({ r, g, b, a });
	});

	m_state.set_function("UI_Button", [&](float x, float y, float width, float height, std::string text, sol::function callback)
	{
		glm::vec2 absolutePosition = uiManager.GetAbsolutePosition(width, height, UIFlag::UIFlag_None);

		absolutePosition.x += x;
		absolutePosition.y += y;

		UIObject* uiObject = new UIButtonObject(
			{ absolutePosition.x, absolutePosition.y, 0 },
			{ width, height, 1 }
		);

		uiObject->active = uiManager.GetActive();
		uiObject->backgroundColor = uiManager.GetBackgroundColor();
		uiObject->fontSize = uiManager.GetFontSize();
		uiObject->fontColor = uiManager.GetFontColor();
		uiObject->text = text;
		uiObject->onClickFunction = callback;
		uiManager.AddObject(uiObject);
	});

	m_state.set_function("UI_Image", [&](const std::string& imagePath, const std::string& hoverImagePath, float x, float y, float width, float height, float scaleX, float scaleY, float alpha, sol::function callback)
	{
		glm::vec2 absolutePosition = uiManager.GetAbsolutePosition(width, height, UIFlag::UIFlag_None);

		absolutePosition.x += x;
		absolutePosition.y += y;

		UIImageObject* uiObject = new UIImageObject();
		uiObject->active = uiManager.GetActive();

		if (Image* image = dynamic_cast<Image*>(Project::GetActive()->GetAsset(imagePath)))
			uiObject->image = image;

		if (Image* image = dynamic_cast<Image*>(Project::GetActive()->GetAsset(hoverImagePath)))
			uiObject->hoverImage = image;

		uiObject->imageSize.x = scaleX;
		uiObject->imageSize.y = scaleY;
		uiObject->position = glm::vec3{ absolutePosition.x, absolutePosition.y, 0.0f };
		uiObject->scale = glm::vec3{ width, height, 1 };
		uiObject->backgroundColor = uiManager.GetBackgroundColor();
		uiObject->fontSize = uiManager.GetFontSize();
		uiObject->fontColor = uiManager.GetFontColor();
		uiObject->color.a = alpha;
		uiObject->onClickFunction = callback;
		uiManager.AddObject(uiObject);
	});

	m_state.set_function("UI_Slider", [&](float x, float y, float width, float height, int value)
	{
		UIObject* uiObject = new UIObject();
		uiObject->position = glm::vec3{ x, y, 0.0f };
		uiObject->scale = glm::vec3{ width, height, 1.0f };
		uiManager.AddObject(uiObject);

		UIObject* uiText = new UIObject();
		uiText->position = glm::vec3{ x, y, 0.0f };
		uiText->fontSize = uiManager.GetFontSize();
		uiText->fontColor = uiManager.GetFontColor();
		uiText->text = std::to_string(value);
		uiManager.AddObject(uiText);
	});

	m_state.set_function("UI_Text", [&](std::string text, float x, float y, float size)
	{
		UIRect offsetRect = uiManager.GetOffsetRect();

		glm::vec2 absolutePosition = offsetRect.position;

		absolutePosition.x += x;
		absolutePosition.y += y;

		UIObject* uiObject = new UIText(
			glm::vec3{ absolutePosition.x, absolutePosition.y, 0.0f }
		);
		uiObject->fontSize = size;
		uiObject->fontColor = uiManager.GetFontColor();
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
			float fontWidth = uiManager.GetFontSize() * 0.6f;

			while (true)
			{
				sol::optional<sol::object> exist = keyTable[index];

				if (exist == sol::nullopt)
					return;

				sol::object key = keyTable[index];
				std::string text = key.as<std::string>();

				UIObject* uiObject = new UIObject();
				uiObject->position = glm::vec3{ offsetX, y, 0.0f };
				uiObject->fontSize = uiManager.GetFontSize();
				uiObject->fontColor = uiManager.GetFontColor();

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