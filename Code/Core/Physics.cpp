#include "Physics.h"

Physics::Physics()
  : m_world(b2Vec2(0.0f, -15.0f))
  , m_velocityIterations(6)
  , m_positionIterations(2)
{
  m_world.SetContactListener(&m_contactListener);
}

bool inside(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 p) {
    return (cp2.x-cp1.x)*(p.y-cp1.y) > (cp2.y-cp1.y)*(p.x-cp1.x);
}

b2Vec2 intersection(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 s, b2Vec2 e) {
    b2Vec2 dc( cp1.x - cp2.x, cp1.y - cp2.y );
    b2Vec2 dp( s.x - e.x, s.y - e.y );
    float n1 = cp1.x * cp2.y - cp1.y * cp2.x;
    float n2 = s.x * e.y - s.y * e.x;
    float n3 = 1.0 / (dc.x * dp.y - dc.y * dp.x);
    return b2Vec2( (n1*dp.x - n2*dc.x) * n3, (n1*dp.y - n2*dc.y) * n3);
}

bool findIntersectionOfFixtures(b2Fixture* fA, b2Fixture* fB, std::vector<b2Vec2>& outputVertices)
{
    //currently this only handles polygon vs polygon
    if ( fA->GetShape()->GetType() != b2Shape::e_polygon ||
         fB->GetShape()->GetType() != b2Shape::e_polygon )
        return false;

    b2PolygonShape* polyA = (b2PolygonShape*)fA->GetShape();
    b2PolygonShape* polyB = (b2PolygonShape*)fB->GetShape();

    //fill subject polygon from fixtureA polygon
    for (int i = 0; i < polyA->m_count; i++)
        outputVertices.push_back( fA->GetBody()->GetWorldPoint( polyA->m_vertices[i] ) );

    //fill clip polygon from fixtureB polygon
    std::vector<b2Vec2> clipPolygon;
    for (int i = 0; i < polyB->m_count; i++)
        clipPolygon.push_back( fB->GetBody()->GetWorldPoint( polyB->m_vertices[i] ) );

    b2Vec2 cp1 = clipPolygon[clipPolygon.size()-1];
    for (int j = 0; j < clipPolygon.size(); j++) {
        b2Vec2 cp2 = clipPolygon[j];
        if ( outputVertices.empty() )
            return false;
        std::vector<b2Vec2> inputList = outputVertices;
        outputVertices.clear();
        b2Vec2 s = inputList[inputList.size() - 1]; //last on the input list
        for (int i = 0; i < inputList.size(); i++) {
            b2Vec2 e = inputList[i];
            if (inside(cp1, cp2, e)) {
                if (!inside(cp1, cp2, s)) {
                    outputVertices.push_back( intersection(cp1, cp2, s, e) );
                }
                outputVertices.push_back(e);
            }
            else if (inside(cp1, cp2, s)) {
                outputVertices.push_back( intersection(cp1, cp2, s, e) );
            }
            s = e;
        }
        cp1 = cp2;
    }

    return !outputVertices.empty();
}

b2Vec2 ComputeCentroid(std::vector<b2Vec2> vs, float& area)
{
    int count = (int)vs.size();
    b2Assert(count >= 3);

    b2Vec2 c;
    c.Set(0.0f, 0.0f);
    area = 0.0f;

    // pRef is the reference point for forming triangles.
    // Its location doesnt change the result (except for rounding error).
    b2Vec2 pRef(0.0f, 0.0f);

    const float inv3 = 1.0f / 3.0f;

    for (int32 i = 0; i < count; ++i)
    {
        // Triangle vertices.
        b2Vec2 p1 = pRef;
        b2Vec2 p2 = vs[i];
        b2Vec2 p3 = i + 1 < count ? vs[i+1] : vs[0];

        b2Vec2 e1 = p2 - p1;
        b2Vec2 e2 = p3 - p1;

        float D = b2Cross(e1, e2);

        float triangleArea = 0.5f * D;
        area += triangleArea;

        // Area weighted centroid
        c += triangleArea * inv3 * (p1 + p2 + p3);
    }

    // Centroid
    if (area > b2_epsilon)
        c *= 1.0f / area;
    else
        area = 0;
    return c;
}

void Physics::Update(float deltaTime)
{
  std::set<fixturePair>::iterator it = m_contactListener.m_fixturePairs.begin();
  std::set<fixturePair>::iterator end = m_contactListener.m_fixturePairs.end();

  while (it != end) {

    //fixtureA is the fluid
    b2Fixture* fixtureA = it->first;
    b2Fixture* fixtureB = it->second;

    float density = fixtureA->GetDensity();

    std::vector<b2Vec2> intersectionPoints;
    if ( findIntersectionOfFixtures(fixtureA, fixtureB, intersectionPoints) ) {

        //find centroid
        float area = 0;
        b2Vec2 centroid = ComputeCentroid( intersectionPoints, area);
        b2Vec2 gravity( 0, -10.0f);

        //apply buoyancy force (fixtureA is the fluid)
        float displacedMass = fixtureA->GetDensity() * area;
        b2Vec2 buoyancyForce = displacedMass * -gravity;
        fixtureB->GetBody()->ApplyForce( buoyancyForce, centroid, true );
    }

    ++it;
}

	m_world.Step(deltaTime, m_velocityIterations, m_positionIterations);
}

b2Body* Physics::CreateBoxBody(float x, float y, float width, float height, bool isDynamic, bool isSensor)
{
  b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);

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

  b2PolygonShape shape;
	shape.SetAsBox(width / 2, height / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
  fixtureDef.isSensor = isSensor;

  body->SetFixedRotation(true);
	body->CreateFixture(&fixtureDef);

  return body;
}
