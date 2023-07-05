#pragma once

#include "Graphics/Renderer.h"

class OpenGLRenderer : public Renderer
{
public:
	virtual void Initialize() override;
	virtual void Draw(Sprite& sprite, const glm::mat4& modelMatrix) override;
	virtual void DrawLine(const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& color) override;
	virtual void DrawLines(float lines[], int n, const glm::vec4& color = glm::vec4(1)) override;
	virtual void DrawRect(b2Body* body, const Camera& camera) override;
	virtual void DrawCircle(const glm::vec2& position, float radius, bool filled = true) override;

private:
	Shader m_basicShader;
	Shader m_spriteShader;

	unsigned int m_quadVAO;
	unsigned int m_quadVBO;
	unsigned int m_lineVAO;
	unsigned int m_lineVBO;
};