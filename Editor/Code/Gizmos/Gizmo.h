#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Graphics/Renderer.h"
#include "Actionable.h"

class Editor2D;

class Gizmo
{
public:
	Gizmo(Editor2D& editor);
	void Update(float dt);
	void Render(Renderer& renderer, const glm::vec2& position);
	virtual bool OnPicking2D(const glm::vec2& cursorPosition);

protected:
	Editor2D& m_editor;
	glm::vec2 m_position;
	std::vector<std::unique_ptr<Actionable>> m_actionables;
};
