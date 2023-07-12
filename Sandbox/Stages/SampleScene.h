#pragma once

#include "Core/Scene.h"
#include "Core/Input.h"

class SampleScene : public Scene
{
	virtual void Initialize() override
	{
		std::cout << "Sample Scene was initialized..." << std::endl;
	}

	virtual void Update(float deltaTime) override
	{
	}
};