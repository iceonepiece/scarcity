#pragma once

#include <iostream>
#include <filesystem>
#include <set>
#include <bitset>
#include <vector>
#include <box2d/box2d.h>
#include "ContactListener.h"
#include "Components/TransformComponent.h"
#include "Components/BoxCollider2DComponent.h"
#include "Components/Rigidbody2dComponent.h"
#include "Components/CircleCollider2DComponent.h"

enum PhysicsLayer : uint16_t
{
	Layer_None,
	Layer_Platform,
	Layer_Player,
	Layer_Enemy
};

struct Collision2D
{
	std::string name;
	std::string tag;
	uint16_t layer;
	float x;
	float y;
	float lifetime = -1.0f;
};

struct BoxCollision2D : public Collision2D
{
	float width;
	float height;
};

struct CircleCollision2D : public Collision2D
{
	float radius;
};

constexpr int MAX_COLLISION_LAYERS = 16;

class Physics
{
public:
	Physics();
	~Physics();

	void Update(float deltaTime);
	b2Body* CreateCircleBody(b2Vec2 position, float radius, FixtureData* fixtureData, bool isSensor = false);
	b2Body* CreateBodyWithFixture(b2Vec2 position, b2Vec2 size, FixtureData* fixtureData, bool isDynamic = false, bool isSensor = false, PhysicsLayer layer = Layer_None, b2Vec2 center = {0, 0});
	b2Body* CreateBoxBody(Entity& entity, float x, float y, float width, float height, bool isDynamic = false, bool isSensor = false, float gravityScale = 1.0f);
	b2Body* CreateStaticBox(b2Vec2 position, b2Vec2 size, float angle, FixtureData* fixtureData);
	void CreateFixtureDef(b2Body* body, b2Vec2 size, b2Vec2 offset, bool isSensor, FixtureData* fixtureData);
	
	b2FixtureDef CreateBoxFixtureDef();
	b2FixtureDef CreateCircleFixtureDef();

	b2FixtureDef CreateFixtureDef(b2Shape& shape, Collider2DComponent& collider, uint16_t layer = 0);
	b2PolygonShape CreateBoxShape(TransformComponent& transform, BoxCollider2DComponent& bc2d);
	b2CircleShape CreateCircleShape(TransformComponent& transform, CircleCollider2DComponent& cc2d);

	FixtureData* CreateFixtureData(Entity& entity);

	b2FixtureDef CreateBoxCollider2DFixture(Entity& entity, TransformComponent& transform, Rigidbody2DComponent& rb2d, BoxCollider2DComponent& bc2d);
	b2FixtureDef CreateCircleCollider2DFixture(Entity& entity, TransformComponent& transform, Rigidbody2DComponent& rb2d, CircleCollider2DComponent& cc2d);

	b2Body* CreateBody(TransformComponent& transform, Rigidbody2DComponent& rb2d);
	void InitializePhysicsEntity(Entity& entity, TransformComponent& transform, Rigidbody2DComponent& rb2d);
	void DestroyPhysicsEntity(Rigidbody2DComponent& rb2d);

	static bool Serialize(const std::filesystem::path& path);
	static bool Deserialize(const std::filesystem::path& path);

	static std::array<std::string, MAX_COLLISION_LAYERS>& GetLayers() { return s_layers; }
	static std::array<std::bitset<MAX_COLLISION_LAYERS>, MAX_COLLISION_LAYERS>& GetCollisionMatrix() { return s_collisionMatrix; }

private:
	b2World m_world;
	ContactListener m_contactListener;
	int32 m_velocityIterations;
	int32 m_positionIterations;

	std::vector<FixtureData*> m_fixtureDatum;

	std::unordered_map<PhysicsLayer, uint16_t> m_maskMap;

	std::array<std::bitset<MAX_COLLISION_LAYERS>, MAX_COLLISION_LAYERS> m_collisionMatrix;

	static std::array<std::bitset<MAX_COLLISION_LAYERS>, MAX_COLLISION_LAYERS> s_collisionMatrix;
	static std::array<std::string, MAX_COLLISION_LAYERS> s_layers;
};
