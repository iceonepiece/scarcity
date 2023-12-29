#pragma once

#include "Graphics/Renderer.h"
#include "OpenGLShader.h"
#include "Shapes/Shape2D.h"
#include "OpenGLFontSystem.h"

class OpenGLRenderer : public Renderer
{
public:
	virtual void Initialize() override;
	virtual void Draw(Sprite& sprite, const glm::mat4& modelMatrix) override;
	virtual void DrawSprite(Sprite& sprite, const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }) override;
	virtual void DrawQuad(const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }) override;
	virtual void DrawQuad2D(const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }) override;
	virtual void DrawQuad2D(const Quad2D& quad) override;
	virtual void DrawQuadUI(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color) override;
	virtual void DrawLine(const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& color) override;
	virtual void DrawLines(float lines[], int n, const glm::vec4& color = glm::vec4(1)) override;
	virtual void DrawRect(b2Body* body, const Camera& camera) override;
	virtual void DrawRect(const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }, float thickness = 1.0f) override;
	virtual void DrawCircle(const glm::vec2& position, float radius) override;
	virtual void DrawCircle2D(const Circle2D& circle, float thickness = 1.0f) override;
	virtual void DrawText(const std::string& text, const glm::vec2& position, float scale, const glm::vec4& color) override;

	virtual std::unique_ptr<Texture> LoadTexture(const std::string & name, const char* filename, bool alpha) override;

	virtual void Clear(const glm::vec4& color) override;
	virtual void PreRender(bool font = false) override;
	virtual void PostRender(bool font = false) override;

	virtual std::unique_ptr<Framebuffer> CreateFramebuffer() override;

private:
	OpenGLShader m_basicShader;
	OpenGLShader m_spriteShader;
	OpenGLShader m_uiShader;
	OpenGLShader m_circleShader;

	GLuint m_quadVAO;
	GLuint m_quadVBO;
	GLuint m_lineVAO;
	GLuint m_lineVBO;
	GLuint m_circleVAO;
	GLuint m_circleVBO;

	OpenGLFontSystem m_fontSystem;
};