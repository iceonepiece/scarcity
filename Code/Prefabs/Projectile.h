#pragma once

#include <box2d/box2d.h>

#include "../Core/Entity.h"
#include "../Core/Scene.h"
#include "../Components/Collider2DComponent.h"

class Projectile
{
public:
	Projectile(Entity entity, Scene *scene, b2Vec2 position, float width, float height, float speed)
	{
		b2Body* projectileBody = scene->m_physics.CreateBoxBody(position.x, position.y, width, height, true, true, 0);
		projectileBody->SetLinearVelocity(b2Vec2(speed, 0));
		entity.AddComponent<Collider2DComponent>(projectileBody);
	}
};