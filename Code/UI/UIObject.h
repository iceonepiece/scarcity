#pragma once

#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include "Input/Input.h"
#include "Graphics/Image.h"
#include "Math/Math.h"
#include "Graphics/Renderer.h"
#include "UI/UIBase.h"

class UIObject
{
public:
	UIObject() = default;

	UIObject(const glm::vec3& position, const glm::vec3& scale = glm::vec3{1.0f})
		: position(position)
		, scale(scale)
	{}

	virtual ~UIObject() = default;

	virtual void Render(Renderer& renderer) {}

	bool active = true;

	virtual UIType Type()
	{
		return UIType_Box;
	}

	glm::vec3 position{ 0.0f };
	glm::vec3 scale{ 1.0f };
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
	glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 fontColor{ 0.1f, 0.1f, 0.1f, 1.0f };

	glm::vec4 backgroundColor { 0, 0, 0, 1 };
	glm::vec4 onHoverColor{ 0.8f, 0.8f, 0.1f, 1.0f };

	UIFlag flags = UIFlag_None;

	std::string text;
	int fontSize = 18;

	UIRect parentRect;

	glm::vec3 absolutePosition;
	glm::vec3 absoluteScale;
	glm::vec3 absoluteRotation;

	sol::function onHoverFunction;
	sol::function onClickFunction;

	Image* image = nullptr;

	virtual void HandleInput(Input& input) { color = backgroundColor; }
};

class UIText : public UIObject
{
public:
	UIText() = default;

	UIText(const glm::vec3& position, const glm::vec3& scale = glm::vec3{ 1.0f })
		: UIObject(position, scale)
	{}

	virtual UIType Type() override
	{
		return UIType_Text;
	}
};

class UIImageObject : public UIObject
{
public:
	~UIImageObject() = default;

	Image* currentImage = nullptr;
	Image* hoverImage = nullptr;
	glm::vec2 imageSize{ 1.0f, 1.0f };

	virtual UIType Type() override
	{
		return UIType_Image;
	}

	virtual void Render(Renderer& renderer) override
	{
		glm::vec2 screenSize = renderer.GetScreenSize();
		if (currentImage != nullptr)
		{
			glm::vec2 imagePos = position;
			imagePos.y = screenSize.y - position.y;

			glm::vec2 imageScale = scale;
			imageScale.x *= imageSize.x;
			imageScale.y *= imageSize.y;
			renderer.DrawSprite(currentImage->GetSprites()[0], imagePos, imageScale, rotation.z, color);
		}
	}

	virtual void HandleInput(Input& input) override
	{
		currentImage = image;

		if (!active)
			return;

		if (Math::Contains(position, scale, input.GetCursorPosition()))
		{
			if (hoverImage != nullptr)
				currentImage = hoverImage;

			if (onHoverFunction.valid())
				onHoverFunction();

			if (input.GetMouseButtonDown(Mouse::ButtonLeft))
			{
				if (onClickFunction.valid())
					onClickFunction();
			}
		}
	}
};

class UIButtonObject : public UIObject
{
public:
	UIButtonObject() = default;

	UIButtonObject(const glm::vec3& position, const glm::vec3& scale = glm::vec3{ 1.0f })
		: UIObject(position, scale)
	{}

	~UIButtonObject() = default;

	virtual UIType Type() override
	{
		return UIType_Button;
	}

	virtual void HandleInput(Input& input) override
	{
		color = backgroundColor;

		if (!active)
			return;

		if (Math::Contains(position, scale, input.GetCursorPosition()))
		{
			color.x *= 1.5f;
			color.y *= 1.5f;
			color.z *= 1.5f;

			if (onHoverFunction.valid())
				onHoverFunction();

			if (input.GetMouseButtonDown(Mouse::ButtonLeft))
			{
				if (onClickFunction.valid())
					onClickFunction();
			}
		}
	}
};

