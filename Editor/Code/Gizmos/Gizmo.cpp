#include "Gizmo.h"
#include "../Editor2D.h"

Gizmo::Gizmo(Editor2D& editor)
	:m_editor(editor)
{}

void Gizmo::Update(float dt)
{
	TransformComponent* transform = m_editor.GetEntityTransform();
	if (transform != nullptr)
	{
		for (auto& actionable : m_actionables)
		{
			actionable->SetTransformComponent(transform);
		}
	}
}

bool Gizmo::OnPicking2D(const glm::vec2& cursorPosition)
{
	TransformComponent* transform = m_editor.GetEntityTransform();
	if (transform != nullptr)
	{
		for (auto& actionable : m_actionables)
		{
			if (actionable->IsCursorOn(cursorPosition.x, cursorPosition.y, transform->position))
			{
				std::cout << "IsCursorOn: " << cursorPosition.x << ", " << cursorPosition.y << std::endl;
				actionable->SetStartCursorPosition(cursorPosition);
				m_actor = actionable.get();
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
