#pragma once

#include "UI/UIObject.h"
#include <sol/sol.hpp>
#include <vector>

class UIManager
{
public:
	void CreateUIComponent(sol::table& uiComponent, const UIRect& parentRect = {});

	void Clear();
	void ClearStack()
	{
		m_rectStack.clear();
	}

	void PushRect(const UIRect& rect)
	{
		m_rectStack.push_back(rect);
	}

	void PopRect()
	{
		m_rectStack.pop_back();
	}

	std::vector<std::unique_ptr<UIObject>>& GetObjects()
	{
		return m_objects;
	}

	void AddObject(UIObject* object)
	{
		m_objects.emplace_back(object);
	}

	UIRect GetOffsetRect();

	glm::vec2 GetAbsolutePosition(float width, float height, UIFlag flags);

	void SetActive(bool active)
	{
		m_active = active;
	}

	bool GetActive()
	{
		return m_active;
	}

	void SetScreenSize(const glm::vec2& size)
	{
		m_screenSize = size;
	}

	glm::vec2& GetScreenSize()
	{
		return m_screenSize;
	}

	void SetBackgroundColor(const glm::vec4& color)
	{
		m_backgroundColor = color;
	}

	glm::vec4& GetBackgroundColor()
	{
		return m_backgroundColor;
	}

	void SetFontName(const std::string& name)
	{
		m_fontName = name;
	}

	std::string GetFontName()
	{
		return m_fontName;
	}

	void SetFontColor(const glm::vec4& color)
	{
		m_fontColor = color;
	}

	glm::vec4& GetFontColor()
	{
		return m_fontColor;
	}

	void SetFontSize(float size)
	{
		m_fontSize = size;
	}

	float GetFontSize()
	{
		return m_fontSize;
	}

	void SetFlag(UIFlag flags)
	{
		m_flags = flags;
	}

	UIFlag GetFlag()
	{
		return m_flags;
	}

private:
	std::vector<std::unique_ptr<UIObject>> m_objects;

	bool m_active = true;

	glm::vec4 m_backgroundColor = glm::vec4(1.0f);
	glm::vec4 m_fontColor = glm::vec4(0.0f);
	float m_fontSize = 32.0f;
	UIFlag m_flags = UIFlag_None;

	std::string m_fontName = "";
	std::vector<UIRect> m_rectStack;
	glm::vec2 m_screenSize;
};