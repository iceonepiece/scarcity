#pragma once

#include "EditorCommand.h"
#include "Components/TransformComponent.h"
#include "Components/Relationship.h"
#include <glm/glm.hpp>

class TranslateCommand : public EditorCommand
{
public:
	TranslateCommand(Entity entity, const glm::vec3& oldPosition)
		: m_entity(entity)
		, m_transform(*m_entity.GetComponent<TransformComponent>())
		, m_newPosition(m_transform.position)
		, m_oldPosition(oldPosition)
		, m_diffPosition(m_transform.position - oldPosition)
	{
	}

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
		m_entity.UpdateEntityAndChildren([&](Entity& entity)
		{
			TransformComponent* transform = entity.GetComponent<TransformComponent>();
			transform->position += m_diffPosition;
		});
	}

	virtual void Undo() override
	{
		m_entity.UpdateEntityAndChildren([&](Entity& entity)
		{
			TransformComponent* transform = entity.GetComponent<TransformComponent>();
			transform->position -= m_diffPosition;
		});
	}

private:
	Entity m_entity;
	TransformComponent& m_transform;
	glm::vec3 m_diffPosition;
	glm::vec3 m_newPosition;
	glm::vec3 m_oldPosition;
};