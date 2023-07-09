#pragma once

#include <box2d/box2d.h>
#include "../Physics/Physics.h"

class Car
{
public:
	Car(b2World* m_world)
		: m_speed(50.0f)
	{
		b2PolygonShape chassis;
		b2Vec2 vertices[6]{};
		vertices[0].Set(-1.5f, -0.5f);
		vertices[1].Set(1.5f, -0.5f);
		vertices[2].Set(1.5f, 0.0f);
		vertices[3].Set(0.0f, 0.9f);
		vertices[4].Set(-1.15f, 0.9f);
		vertices[5].Set(-1.5f, 0.2f);
		chassis.Set(vertices, 6);

		b2CircleShape circle;
		circle.m_radius = 0.4f;

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position.Set(0.0f, 1.0f);
		m_car = m_world->CreateBody(&bd);
		m_car->CreateFixture(&chassis, 1.0f);

		b2FixtureDef fd;
		fd.shape = &circle;
		fd.density = 1.0f;
		fd.friction = 0.9f;

		bd.position.Set(-1.0f, 0.35f);
		m_wheel1 = m_world->CreateBody(&bd);
		m_wheel1->CreateFixture(&fd);

		bd.position.Set(1.0f, 0.4f);
		m_wheel2 = m_world->CreateBody(&bd);
		m_wheel2->CreateFixture(&fd);

		b2WheelJointDef jd;
		b2Vec2 axis(0.0f, 1.0f);

		float mass1 = m_wheel1->GetMass();
		float mass2 = m_wheel2->GetMass();

		float hertz = 4.0f;
		float dampingRatio = 0.7f;
		float omega = 2.0f * b2_pi * hertz;

		jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
		jd.motorSpeed = 0.0f;
		jd.maxMotorTorque = 20.0f;
		jd.enableMotor = true;
		jd.stiffness = mass1 * omega * omega;
		jd.damping = 2.0f * mass1 * dampingRatio * omega;
		jd.lowerTranslation = -0.25f;
		jd.upperTranslation = 0.25f;
		jd.enableLimit = true;
		m_spring1 = (b2WheelJoint*)m_world->CreateJoint(&jd);

		jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
		jd.motorSpeed = 0.0f;
		jd.maxMotorTorque = 10.0f;
		jd.enableMotor = false;
		jd.stiffness = mass2 * omega * omega;
		jd.damping = 2.0f * mass2 * dampingRatio * omega;
		jd.lowerTranslation = -0.25f;
		jd.upperTranslation = 0.25f;
		jd.enableLimit = true;
		m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&jd);
	}

	void Render(Camera& camera)
	{
		//Renderer::DrawCircle(m_wheel1, glm::vec4(1, 1, 1, 1), camera);
		//Renderer::DrawCircle(m_wheel2, glm::vec4(1, 1, 1, 1), camera);
	}

private:

	b2Body* m_car;
	b2Body* m_wheel1;
	b2Body* m_wheel2;

	float m_speed;
	b2WheelJoint* m_spring1;
	b2WheelJoint* m_spring2;
};