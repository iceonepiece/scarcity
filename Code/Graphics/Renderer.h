#pragma once

#include <box2d/box2d.h>

#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Texture.h"
#include "Animations/Sprite.h"
#include "Shapes/Shape2D.h"
#include "Components/CameraComponent.h"

class Renderer
{
public:
	static std::unique_ptr<Renderer> Create();

	virtual ~Renderer() = default;

	virtual void Initialize() = 0;
	virtual void Draw(Sprite& sprite, const glm::mat4& modelMatrix) = 0;
	virtual void DrawSprite(Sprite& sprite, const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }) = 0;
	virtual void DrawQuad(const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }) = 0;
	virtual void DrawQuad2D(const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }) = 0;
	virtual void DrawQuad2D(const Quad2D& quad) = 0;
	virtual void DrawQuadUI(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color) = 0;
	virtual void DrawLine(const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& color = glm::vec4(1)) = 0;
	virtual void DrawLines(float lines[], int n, const glm::vec4& color = glm::vec4(1)) = 0;
	virtual void DrawRect(b2Body* body, const Camera& camera) = 0;
	virtual void DrawRect(const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }, float thickness = 1.0f) = 0;
	virtual void DrawCircle(const glm::vec2& position, float radius) = 0;
	virtual void DrawCircle2D(const Circle2D& circle, float thickness = 1.0f) = 0;
	virtual void DrawText(const std::string& text, const glm::vec2& position, float scale, const glm::vec4& color) = 0;


	virtual void CreateFramebuffer() = 0;
	virtual void BindFramebuffer() = 0;
	virtual void UnbindFramebuffer() = 0;
	virtual void RescaleFramebuffer(float width, float height) = 0;

	virtual unsigned int GetFramebufferTextureID() = 0;

	inline void SetScreenSize(int width, int height)
	{
		m_screenSize = { width, height };
	}

	inline float GetScreenSizePercentage() { return m_screenSize.x / m_defaultScreenSize.x; }

	void SetCamera(Camera* camera);
	void SetCamera(CameraComponent camera)
	{
		m_cameraComponent = camera;
	}

	inline void SetProjectionMatrix(const glm::mat4& projection)
	{
		m_projectionMatrix = projection;
	}

	inline void SetViewMatrix(const glm::mat4& view)
	{
		m_viewMatrix = view;
	}

protected:
	Camera *m_camera;
	CameraComponent m_cameraComponent;
	glm::vec2 m_screenSize;
	glm::vec2 m_defaultScreenSize;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	friend class RenderSystem;  
};
