#pragma once

#include "Core/Scene.h"

class SampleScene : public Scene
{
	virtual void Initialize() override
	{
		std::cout << "Sample Scene was initialized..." << std::endl;
	}
};