#pragma once

#include "Gizmo.h"
#include "ActionableRect.h"

class TranslateGizmo : public Gizmo
{
public:
	TranslateGizmo(Editor2D& editor)
		: Gizmo(editor)
	{
		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { 13.0f },
			glm::vec2 { 30.0f },
			glm::vec4 { 0.2, 0.2, 0.7, 0.5 }
		));

		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { m_length / 2, 0.0f },
			glm::vec2 { m_length, m_thickness },
			glm::vec4 { 0.8, 0.2, 0.2, 1.0 }
		));

		m_actionables.push_back(std::make_unique<ActionableRect>(
			glm::vec2 { 0.0f, m_length / 2 },
			glm::vec2 { m_thickness, m_length },
			glm::vec4 { 0.2, 0.8, 0.2, 1.0 }
		));
	}

protected:
	float m_length = 150.0f;
	float m_thickness = 6.0f;


};