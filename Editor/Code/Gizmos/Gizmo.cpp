#include "Gizmo.h"
#include "../EditorLayer.h"
#include <iostream>

Gizmo::Gizmo(EditorLayer& editor)
	:m_editor(editor)
{}

void Gizmo::Update(float dt)
{
	TransformComponent* transform = m_editor.GetEntityTransform();
	
	float zoom = m_editor.GetCamera().GetZoom();

	if (transform != nullptr)
	{
		for (auto& actionable : m_actionables)
		{
			actionable->SetTransformComponent(transform);
			actionable->UpdateTransform(zoom);
		}
	}
}

bool Gizmo::OnPicking2D(const glm::vec2& cursorPosition)
{
	TransformComponent* transform = m_editor.GetEntityTransform();
	if (transform != nullptr)
	{
		for (auto it = m_actionables.rbegin(); it != m_actionables.rend(); ++it)
		{
			if (it->get()->IsCursorOn(cursorPosition.x, cursorPosition.y, transform->position))
			{
				std::cout << "IsCursorOn: " << cursorPosition.x << ", " << cursorPosition.y << std::endl;
				it->get()->SetStartCursorPosition(cursorPosition);
				it->get()->SetEntity(m_editor.GetSelectedEntity());
				it->get()->m_startTransform = *transform;
				m_actor = it->get();
				return true;
			}
		}
	}

	m_actor = nullptr;

	return false;
}

void Gizmo::Render(Renderer& renderer, const glm::vec2& position)
{
	for (auto& actionable : m_actionables)
	{
		actionable->Render(renderer);
	}
}