#pragma once

#include "Gizmo.h"
#include "Actionable.h"
#include "ActionableRect.h"
#include "ActionableCircle.h"

class RotateGizmo : public Gizmo
{
public:
	RotateGizmo(Editor2D& editor)
		: Gizmo(editor)
	{
		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { 0.0f },
			glm::vec2 { 5.0f, m_length },
			glm::vec4 { 0.2, 0.8, 0.2, 1.0 }
		));

		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { 0.0f },
			glm::vec2 { m_length, 5.0f },
			glm::vec4 { 0.8, 0.2, 0.2, 1.0 }
		));

		m_actionables.push_back(std::make_unique<ActionableCircle>(
			glm::vec2 { 0.0f },
			m_length / 2,
			glm::vec4 { 1.0f }
		));
	}

private:
	float m_length = 300.0f;
};