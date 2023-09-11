#include "Core/Window.h"
#include "Platforms/OpenGL/OpenGLWindow.h"

std::unique_ptr<Window> Window::Create(const WindowConfigs& configs)
{
	return std::make_unique<OpenGLWindow>(configs);
}