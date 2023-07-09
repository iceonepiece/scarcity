#pragma once

#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Platforms/OpenGL/OpenGLShader.h"
#include "Camera.h"

struct ParticleProps
{
	glm::vec2 position;
	glm::vec2 velocity, velocityVariation;
	glm::vec4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifeTime = 1.0f;
	int amount = 1;
};

struct Particle
{
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec4 colorBegin, colorEnd;
	float rotation = 0.0f;
	float sizeBegin, sizeEnd;

	float lifeTime = 1.0f;
	float lifeRemaining = 0.0f;

	bool active = false;
};

class ParticleSystem
{
public:
	ParticleSystem();

	static void Init();
	static void Update(float deltaTime);
	static void Render(Camera& camera);
	static void Emit(const ParticleProps& particleProps);

private:
	static std::vector<Particle> m_ParticlePool;
	static uint32_t m_PoolIndex;

	static OpenGLShader m_shader;
	static unsigned int m_VAO;
};
