#pragma once

#include <memory>
#include "Graphics/Renderer.h"

class ServiceLocator
{
public:
	static void SetRenderer(Renderer* renderer)
	{
		s_renderer = std::unique_ptr<Renderer>(renderer);
	}

	static Renderer& GetRenderer() { return *s_renderer; }

private:
	static std::unique_ptr<Renderer> s_renderer;
};
