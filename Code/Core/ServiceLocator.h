#pragma once

#include "Common.h"
#include "Graphics/Renderer.h"

class ServiceLocator
{
public:
	static void RegisterRenderer(Renderer* renderer)
	{
		s_renderer = Unique<Renderer>(renderer);
	}

	static Renderer& GetRenderer() { return *s_renderer; }

private:
	static Unique<Renderer> s_renderer;
};
