#pragma once

#include "Events/Event.h"
#include "Core/Application.h"

class Layer
{
public:
	Layer() = default;
	virtual ~Layer() = default;

	virtual void Update(float deltaTime) {}
	virtual void OnEvent(Event& event) {}
	virtual void RenderImGui() {}

	virtual void Initialize() {}
	virtual void Shutdown() {}
};