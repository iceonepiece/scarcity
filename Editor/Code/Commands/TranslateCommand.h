#pragma once

#include "EditorCommand.h"
#include "Components/TransformComponent.h"
#include <glm/glm.hpp>

class TranslateCommand : public EditorCommand
{
public:
	TranslateCommand(TransformComponent& transform, const glm::vec3& oldPosition)
		: m_transform(transform)
		, m_newPosition(transform.position)
		, m_oldPosition(oldPosition)
	{

	}

	virtual void Execute() override
	{
		m_transform.position = m_newPosition;
	}

	virtual void Redo() override
	{
		m_transform.position = m_newPosition;
		std::cout << "Translate Redo\n";
	}

	virtual void Undo() override
	{
		m_transform.position = m_oldPosition;
	}

private:
	TransformComponent& m_transform;
	glm::vec3 m_newPosition;
	glm::vec3 m_oldPosition;
};