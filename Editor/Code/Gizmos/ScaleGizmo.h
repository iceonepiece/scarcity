#pragma once

#include "Gizmo.h"

class ScaleGizmo : public Gizmo
{
public:
	virtual void Update(float dt) override
	{

	}

	virtual void Render(Renderer& renderer) override
	{
		renderer.DrawQuad(glm::vec2 { 1.0f, 0.0f }, glm::vec2 { 2.0f, 0.05f }, 0.0f, glm::vec4 { 0.8, 0.2, 0.2, 1.0 });
		renderer.DrawQuad(glm::vec2 { 0.0f, 1.0f }, glm::vec2 { 2.0f, 0.05f }, glm::pi<float>() / 2, glm::vec4 { 0.2, 0.8, 0.2, 1.0 });
		renderer.DrawQuad(glm::vec2 { 0.0f, 0.0f }, glm::vec2 { 0.4f, 0.4f }, 0.0f, glm::vec4 { 1.0 });
	}
};