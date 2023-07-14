#pragma once

#include "Core/Scene.h"
#include "Core/Input.h"
#include "Systems/AnimationSystem.h"

class SampleScene : public Scene
{
	virtual void Initialize() override
	{
		std::cout << "Sample Scene was initialized..." << std::endl;
		m_systems.emplace_back(new AnimationSystem(this));
	}

	virtual void Update(float deltaTime) override
	{
	}
};