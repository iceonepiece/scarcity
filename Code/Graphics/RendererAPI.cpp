#include "RendererAPI.h"

Renderer* RendererAPI::s_renderer = nullptr;
glm::vec2 RendererAPI::s_screenSize;
glm::vec2 RendererAPI::s_screenOffset;

void RendererAPI::Initialize(Renderer *renderer)
{
	s_renderer = renderer;
	s_renderer->Initialize();
}

void RendererAPI::Terminate()
{
	delete s_renderer;
}

void RendererAPI::SetScreenSize(int width, int height, int xOffset, int yOffset)
{
	s_screenSize.x = width;
	s_screenSize.y = height;
	s_screenOffset.x = xOffset;
	s_screenOffset.y = yOffset;
}

glm::vec2 RendererAPI::GetScreenSize()
{
	return glm::vec2(s_screenSize.x, s_screenSize.y);
}

Renderer& RendererAPI::GetRenderer()
{
	return *s_renderer;
}
