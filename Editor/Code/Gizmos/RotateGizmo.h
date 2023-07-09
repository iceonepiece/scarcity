#pragma once

#include "Gizmo.h"

class RotateGizmo : public Gizmo
{
public:
	virtual void Update(float dt) override
	{

	}

	virtual void Render(Renderer& renderer) override
	{
		renderer.DrawQuad(glm::vec2 { 0.0f, 0.0f }, glm::vec2 { 4.0f, 0.05f }, 0.0f, glm::vec4 { 0.2, 0.8, 0.2, 1.0 });
		renderer.DrawQuad(glm::vec2 { 0.0f, 0.0f }, glm::vec2 { 4.0f, 0.05f }, glm::pi<float>() / 2, glm::vec4 { 0.8, 0.2, 0.2, 1.0 });
		renderer.DrawCircle(glm::vec2 { 0.0f, 0.0f }, 2.0f, false);
		//renderer.DrawRect()
	}
};