#pragma once

#include <iostream>
#include "../Core/Scriptable.h"

class Player : public Scriptable
{
private:
	PlayerComponent *m_player;
	Collider2DComponent *m_collider;
	TransformComponent *m_transform;
	SpriteAnimatorComponent *m_animator;

public:
	Player(Entity entity)
		: Scriptable(entity)
		, m_player(nullptr)
		, m_collider(nullptr)
		, m_transform(nullptr)
	{}

	virtual void Start()
	{		
		m_player = m_entity.GetComponent<PlayerComponent>();
		m_collider = m_entity.GetComponent<Collider2DComponent>();
		m_transform = m_entity.GetComponent<TransformComponent>();
		m_animator = m_entity.GetComponent<SpriteAnimatorComponent>();
	}

	virtual void Update(float dt)
	{
		m_player->movementState = MS_IDLE;
		m_player->jump = false;

		if (Input::IsKeyHeld(Key::Left))
			m_player->movementState = MS_LEFT;

		if (Input::IsKeyHeld(Key::Right))
			m_player->movementState = MS_RIGHT;

		if (Input::IsKeyPressed(Key::Space))
			m_player->jump = true;

		if ((m_player->direction == 1 && m_player->movementState == MS_LEFT) ||
			(m_player->direction == -1 && m_player->movementState == MS_RIGHT))
		{
			m_player->direction *= -1;
			m_transform->scale.x = m_player->direction;
		}

		//m_animator->fsm->SetBool("walking", m_player->movementState != MS_IDLE);
		//m_animator->fsm->SetBool("jumping", m_player->numGrounds == 0);

	}
};