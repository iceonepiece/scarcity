#include "ParticleSystem.h"
#include "Random.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

unsigned int ParticleSystem::m_VAO = 0;
uint32_t ParticleSystem::m_PoolIndex = 999;
Shader ParticleSystem::m_shader;
std::vector<Particle> ParticleSystem::m_ParticlePool;


ParticleSystem::ParticleSystem()
{

}

void ParticleSystem::Init()
{
	float vertices[] = {
		 -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f,  0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	unsigned int quadVB, quadIB;
	glGenBuffers(1, &quadVB);
	glBindBuffer(GL_ARRAY_BUFFER, quadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0
	};

	glGenBuffers(1, &quadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	m_shader.Compile("Code/Shaders/particle.vert", "Code/Shaders/particle.frag");

	m_ParticlePool.resize(1000);
}

void ParticleSystem::Update(float deltaTime)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.active)
			continue;

		if (particle.lifeRemaining <= 0.0f)
		{
			particle.active = false;
			continue;
		}

		particle.lifeRemaining -= deltaTime;
		particle.position += particle.velocity * deltaTime;
		particle.rotation += 0.01f * deltaTime;
	}
}

void ParticleSystem::Render(Camera& camera)
{
	m_shader.Use();

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, camera.GetPosition());

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);

	glm::mat4 viewProj = projection * view;

	m_shader.SetMatrix4("u_ViewProj", viewProj);

	for (auto& particle : m_ParticlePool)
	{
		if (!particle.active)
			continue;

		float life = particle.lifeRemaining / particle.lifeTime;
		glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);
		//color.a = color.a * life;

		float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.position.x, particle.position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), particle.rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });

		m_shader.SetMatrix4("u_Transform", transform);
		m_shader.SetVector4f("u_Color",color);

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.active = true;
	particle.position = particleProps.position;
	particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.velocity = particleProps.velocity;
	particle.velocity.x += particleProps.velocityVariation.x * (Random::Float() - 0.5f);
	particle.velocity.y += particleProps.velocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.colorBegin = particleProps.colorBegin;
	particle.colorEnd = particleProps.colorEnd;

	particle.lifeTime = particleProps.lifeTime;
	particle.lifeRemaining = particleProps.lifeTime;
	particle.sizeBegin = particleProps.sizeBegin + particleProps.sizeVariation * (Random::Float() - 0.5f);
	particle.sizeEnd = particleProps.sizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}
