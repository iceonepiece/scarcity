#include "Physics.h"
#include "Physics/NullFixtureData.h"
#include "Physics/EntityFixtureData.h"
#include "Components/BoxCollider2DComponent.h"
#include "Components/CircleCollider2DComponent.h"
#include "Components/GridComponent.h"

std::array<std::string, MAX_COLLISION_LAYERS> Physics::s_layers = { "Default" };
std::array<std::bitset<MAX_COLLISION_LAYERS>, MAX_COLLISION_LAYERS> Physics::s_collisionMatrix;

bool Physics::Serialize(const std::filesystem::path& path)
{
	std::ofstream serialized;
	serialized.open(path);

	if (serialized.is_open())
	{
		json physicsJson;
		physicsJson["layers"] = json::array();;

		for (auto& layerName : s_layers)
			physicsJson["layers"].push_back(layerName);

		physicsJson["collisionMatrix"] = json::array();

		for (auto& collisionLayer : s_collisionMatrix)
		{
			physicsJson["collisionMatrix"].push_back(collisionLayer.to_string());
		}

		serialized << physicsJson.dump(4);
	}
	else
	{
		std::cout << "Error opening the file!" << std::endl;
		return false;
	}

	serialized.close();

	return true;
}

bool Physics::Deserialize(const std::filesystem::path& path)
{
	std::ifstream deserialzed(path);

	if (deserialzed.is_open())
	{
		json physicsJson = json::parse(deserialzed);

		if (physicsJson["layers"].is_array())
		{
			for (int i = 0; i < physicsJson["layers"].size(); i++)
			{
				s_layers[i] = physicsJson["layers"][i].get<std::string>();
			}
		}

		s_collisionMatrix.fill(std::bitset<MAX_COLLISION_LAYERS>(0));

		if (physicsJson["collisionMatrix"].is_array())
		{
			for (int i = 0; i < physicsJson["collisionMatrix"].size(); i++)
			{
				s_collisionMatrix[i] = std::bitset<MAX_COLLISION_LAYERS>(physicsJson["collisionMatrix"][i].get<std::string>());
			}
		}
	}
	else
	{
		std::cerr << "Error opening the file!" << std::endl;
		return false;
	}

	deserialzed.close();

	return true;
}

Physics::Physics()
	: m_world(b2Vec2(0.0f, -10.0f))
	, m_velocityIterations(6)
	, m_positionIterations(2)
{
	m_world.SetContactListener(&m_contactListener);
	
	m_maskMap[Layer_None] = Layer_None;
	m_maskMap[Layer_Player] = Layer_Platform;
	m_maskMap[Layer_Enemy] = Layer_Platform;
	m_maskMap[Layer_Platform] = Layer_Player | Layer_Enemy;
}

Physics::~Physics()
{
  for (auto fixtureData : m_fixtureDatum)
  {
    delete fixtureData;
  }
}

void Physics::Update(float deltaTime) 
{
	m_world.Step(deltaTime, m_velocityIterations, m_positionIterations);
}


void Physics::CreateFixtureDef(b2Body* body, b2Vec2 size, b2Vec2 offset, bool isSensor, FixtureData* fixtureData)
{
	b2PolygonShape shape;
	shape.SetAsBox(size.x / 2, size.y / 2, offset, 0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = isSensor ? 0.0f : 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = isSensor;

	m_fixtureDatum.push_back(fixtureData);
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);

	body->CreateFixture(&fixtureDef);
}

b2Body* Physics::CreateCircleBody(b2Vec2 position, float radius, FixtureData* fixtureData, bool isSensor)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = position;
	bodyDef.gravityScale = 1.0f;

	b2Body* body = m_world.CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_radius = radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = isSensor;

	if (fixtureData)
	{
		m_fixtureDatum.push_back(fixtureData);
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
	}

	body->SetFixedRotation(true);
	body->CreateFixture(&fixtureDef);

	return body;
}

b2Body* Physics::CreateStaticBox(b2Vec2 position, b2Vec2 size, float angle, FixtureData* fixtureData)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position = position;
	bodyDef.angle = angle;

	b2Body* body = m_world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(size.x / 2, size.y / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = false;

	if (fixtureData)
	{
		m_fixtureDatum.push_back(fixtureData);
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
	}

	body->SetFixedRotation(true);
	body->CreateFixture(&fixtureDef);

	return body;
}

b2Body* Physics::CreateBodyWithFixture(b2Vec2 position, b2Vec2 size, FixtureData* fixtureData, bool isDynamic, bool isSensor, PhysicsLayer layer, b2Vec2 center)
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(position.x, position.y);

	if (isDynamic)
	{
		bodyDef.type = b2_dynamicBody;
		bodyDef.gravityScale = 1.0f;
	}
	else
	{
		bodyDef.type = b2_staticBody;
	}

	b2Body* body = m_world.CreateBody(&bodyDef);
	body->SetFixedRotation(true);

	b2PolygonShape shape;
	shape.SetAsBox(size.x / 2, size.y / 2, center, 0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = isSensor ? 0.0f : 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = isSensor;
	//fixtureDef.filter.categoryBits = layer;
	//fixtureDef.filter.maskBits = m_maskMap[layer];
	
	if (fixtureData)
	{
		m_fixtureDatum.push_back(fixtureData);
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
	}

	body->CreateFixture(&fixtureDef);

	return body;
}

b2Body* Physics::CreateBoxBody(Entity& entity, float x, float y, float width, float height, bool isDynamic, bool isSensor, float gravityScale)
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);

	if (isDynamic)
	{
		bodyDef.type = b2_dynamicBody;
		bodyDef.gravityScale = gravityScale;
	}
	else
	{
		bodyDef.type = b2_staticBody;
	}

	b2Body* body = m_world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(width / 2, height / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = isSensor;

	FixtureData* fixtureData = new NullFixtureData(entity);
	m_fixtureDatum.push_back(fixtureData);
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);

	body->SetFixedRotation(true);
	body->CreateFixture(&fixtureDef);

	return body;
}

b2FixtureDef Physics::CreateFixtureDef(b2Shape& shape, Collider2DComponent& collider)
{
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = collider.density;
	fixtureDef.friction = collider.friction;
	fixtureDef.isSensor = collider.isTrigger;

	return fixtureDef;
}

b2PolygonShape Physics::CreateBoxShape(TransformComponent& transform, BoxCollider2DComponent& bc2d)
{
	glm::vec2 absoluteScale{ std::abs(transform.scale.x), std::abs(transform.scale.y) };

	b2PolygonShape boxShape;
	boxShape.SetAsBox(
		bc2d.size.x * absoluteScale.x / 2,
		bc2d.size.y * absoluteScale.y / 2,
		b2Vec2(bc2d.offset.x * absoluteScale.x, bc2d.offset.y * absoluteScale.y),
		0.0f
	);

	return boxShape;
}

b2CircleShape Physics::CreateCircleShape(TransformComponent& transform, CircleCollider2DComponent& cc2d)
{
	b2CircleShape circleShape;
	circleShape.m_p.Set(cc2d.offset.x, cc2d.offset.y);
	circleShape.m_radius = transform.scale.x * cc2d.radius;

	return circleShape;
}


b2FixtureDef Physics::CreateBoxCollider2DFixture(Entity& entity, TransformComponent& transform, Rigidbody2DComponent& rb2d, BoxCollider2DComponent& bc2d)
{
	glm::vec2 absoluteScale{ std::abs(transform.scale.x), std::abs(transform.scale.y) };

	b2PolygonShape boxShape = CreateBoxShape(transform, bc2d);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = bc2d.density;
	fixtureDef.friction = bc2d.friction;
	fixtureDef.isSensor = bc2d.isTrigger;

	return fixtureDef;
}

b2FixtureDef Physics::CreateCircleCollider2DFixture(Entity& entity, TransformComponent& transform, Rigidbody2DComponent& rb2d, CircleCollider2DComponent& cc2d)
{
	b2CircleShape circleShape;
	circleShape.m_p.Set(cc2d.offset.x, cc2d.offset.y);
	circleShape.m_radius = transform.scale.x * cc2d.radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = cc2d.density;
	fixtureDef.friction = cc2d.friction;
	fixtureDef.isSensor = cc2d.isTrigger;
	
	return fixtureDef;
}

void Physics::InitializePhysicsEntity(Entity& entity, TransformComponent& transform, Rigidbody2DComponent& rb2d)
{
	b2BodyDef bodyDef;
	bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.type);
	bodyDef.position.Set(transform.position.x, transform.position.y);
	bodyDef.angle = transform.rotation.z;
	bodyDef.gravityScale = rb2d.gravityScale;

	b2Body* body = m_world.CreateBody(&bodyDef);
	body->SetFixedRotation(rb2d.fixedRotation);
	rb2d.body = body;

	if (BoxCollider2DComponent* ptr = entity.GetComponent<BoxCollider2DComponent>())
	{
		auto& bc2d = *ptr;

		b2PolygonShape boxShape = CreateBoxShape(transform, bc2d);
		b2FixtureDef fixtureDef = CreateFixtureDef(boxShape, bc2d);

		FixtureData* fixtureData = new EntityFixtureData(entity);
		fixtureData->m_tag = entity.GetComponent<BaseComponent>()->tag;
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
		rb2d.fixtureData = fixtureData;

		body->CreateFixture(&fixtureDef);
	}

	if (CircleCollider2DComponent* ptr = entity.GetComponent<CircleCollider2DComponent>())
	{
		auto& cc2d = *ptr;

		b2CircleShape circleShape = CreateCircleShape(transform, cc2d);
		b2FixtureDef fixtureDef = CreateFixtureDef(circleShape, cc2d);

		FixtureData* fixtureData = new EntityFixtureData(entity);
		fixtureData->m_tag = entity.GetComponent<BaseComponent>()->tag;
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
		rb2d.fixtureData = fixtureData;

		body->CreateFixture(&fixtureDef);
	}

	if (GridComponent* ptr = entity.GetComponent<GridComponent>())
	{
		auto& grid = *ptr;

		for (int i = 0; i < grid.polygons.size(); i++)
		{
			int edgeSize = grid.polygons[i].size();

			std::cout << "edgeSize: " << edgeSize << '\n';

			b2Vec2* vs = new b2Vec2[edgeSize];

			for (int j = 0; j < edgeSize; j++)
			{
				int x = grid.polygons[i][j].startCell.first;
				int y = grid.polygons[i][j].startCell.second;
				EdgeOnCell onCell = grid.polygons[i][j].onCell;

				float pointX = x;
				float pointY = y;

				if (onCell == EdgeOnCell::Left)
				{
					pointY++;
				}
				else if (onCell == EdgeOnCell::Right)
				{
					pointX++;
				}
				else if (onCell == EdgeOnCell::Top)
				{
					pointX++;
					pointY++;
				}

				vs[j].Set(pointX, pointY);
				std::cout << "Point: " << j << " - " << pointX << "," << pointY << '\n';
			}

			b2ChainShape chain;
			chain.CreateLoop(vs, edgeSize);

			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.0f;
			fixtureDef.isSensor = false;
			fixtureDef.shape = &chain;

			FixtureData* fixtureData = new EntityFixtureData(entity);
			fixtureData->m_tag = entity.GetComponent<BaseComponent>()->tag;
			fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(fixtureData);
			rb2d.fixtureData = fixtureData;

			body->CreateFixture(&fixtureDef);

			delete[] vs;
		}

	}
}

void Physics::DestroyPhysicsEntity(Rigidbody2DComponent& rb2d)
{
	std::vector<FixtureData*> pendingList;

	if (b2Body* body = (b2Body*)rb2d.body)
	{
		auto fixture = body->GetFixtureList();

		while (fixture)
		{
			if (FixtureData* fixtureData = (FixtureData*)fixture->GetUserData().pointer)
				pendingList.push_back((FixtureData*)fixture->GetUserData().pointer);

			fixture = fixture->GetNext();
		}

		m_world.DestroyBody(body);
	}

	for (auto fixtureData : pendingList)
		delete fixtureData;
}