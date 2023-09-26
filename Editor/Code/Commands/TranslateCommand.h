#pragma once

#include "EditorCommand.h"
#include "Components/TransformComponent.h"
#include <glm/glm.hpp>

class TranslateCommand : public EditorCommand
{
public:
	TranslateCommand(TransformComponent& transform, const glm::vec3& position)
		: m_transform(transform)
		, m_position(position)
	{

	}

	virtual void Do() override
	{
		m_transform.position += m_position;
	}

	virtual void Undo() override
	{
		m_transform.position -= m_position;
	}

private:
	TransformComponent& m_transform;
	glm::vec3 m_position;
};