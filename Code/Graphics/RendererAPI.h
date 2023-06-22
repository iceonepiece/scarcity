#pragma once

#include <glm/glm.hpp>
#include "../Animations/Sprite.h"
#include "../Core/Camera.h"
#include "Renderer.h"

class RendererAPI
{
public:
	static void Initialize(Renderer *renderer);
	static void Terminate();
	static Renderer& GetRenderer();
	static void SetScreenSize(int width, int height, int xOffset, int yOffset);
	static glm::vec2 GetScreenSize();

private:
	static Renderer *s_renderer;
	static glm::vec2 s_screenSize;
	static glm::vec2 s_screenOffset;
};