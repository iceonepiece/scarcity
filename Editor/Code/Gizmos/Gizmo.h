#pragma once

#include <glm/glm.hpp>
#include "Graphics/Renderer.h"

class Gizmo
{
public:
	virtual void Update(float dt) = 0;
	virtual void Render(Renderer& renderer) = 0;
	//virtual void OnEvent(const Event& event) = 0;

protected:
	glm::vec2 m_position;
};
