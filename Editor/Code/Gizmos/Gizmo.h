#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Graphics/Renderer.h"
#include "Actionable.h"

class EditorLayer;

class Gizmo
{
public:
	Gizmo(EditorLayer& editor);
	virtual void Update(float dt);
	void Render(Renderer& renderer, const glm::vec2& position);
	virtual bool OnPicking2D(const glm::vec2& cursorPosition);
	
	virtual bool OnDragging(float x, float y)
	{
		if (m_actor != nullptr)
		{
			return m_actor->OnDragging(x, y);
		}

		return false;
	}

	virtual void OnDraggingStart() {}
	virtual void OnDraggingEnd() {}

protected:
	EditorLayer& m_editor;
	glm::vec2 m_position;
	Actionable* m_actor = nullptr;
	std::vector<std::unique_ptr<Actionable>> m_actionables;
};
