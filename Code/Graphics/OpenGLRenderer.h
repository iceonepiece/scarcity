#pragma once

#include "Renderer.h"

class OpenGLRenderer : public Renderer
{
public:
	virtual void Initialize() override;
	virtual void Draw(Sprite& sprite, const glm::mat4& modelMatrix, Camera& camera) override;

private:
	Shader m_spriteShader;
	unsigned int m_VBO;
	unsigned int m_VAO;
};