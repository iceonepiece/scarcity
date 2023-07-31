#pragma once

#include <box2d/box2d.h>

#include "GraphicsData.h"
#include "Shader.h"
#include "Core/Camera.h"
#include "UIs/UI.h"
#include "Graphics/Texture.h"
#include "Animations/Sprite.h"
#include "Shapes/Shape2D.h"
#include "Components/Components.h"

class Renderer
{
public:
	virtual void Initialize() = 0;
	virtual void Draw(Sprite& sprite, const glm::mat4& modelMatrix) = 0;
	virtual void DrawQuad(const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }) = 0;
	virtual void DrawQuad2D(const glm::vec2& position, const glm::vec2& scale, float angle = 0.0f, glm::vec4 color = glm::vec4{ 1.0f }) = 0;
	virtual void DrawQuad2D(const Quad2D& quad) = 0;
	virtual void DrawQuadUI(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color, UIAlignment alignment = UIAlignment::NONE) = 0;
	virtual void DrawLine(const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& color = glm::vec4(1)) = 0;
	virtual void DrawLines(float lines[], int n, const glm::vec4& color = glm::vec4(1)) = 0;
	virtual void DrawRect(b2Body* body, const Camera& camera) = 0;
	virtual void DrawCircle(const glm::vec2& position, float radius) = 0;
	virtual void DrawCircle2D(const Circle2D& circle, float thickness = 1.0f) = 0;

	inline void SetScreenSize(int width, int height)
	{
		m_screenSize.x = width;
		m_screenSize.y = height;
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

  //static void Init();
  //static void DrawQuad(b2Body* body, Camera& camera);
  //static void DrawCircle(b2Body* body, glm::vec4 color, Camera& camera);
  //static void DrawQuadUI(glm::vec2 position, glm::vec2 scale, glm::vec4 color, UIAlignment alignment = UIAlignment::NONE);
  //static void DrawCircle(glm::vec2 position, float radius, glm::vec4 color, Camera& camera);
  //static void DrawSprite(Sprite& sprite, glm::vec2 position, glm::vec2 scale, Camera& camera);
  //static void SetScreenSize(int width, int height, int xOffset, int yOffset);
  //static glm::vec2 GetScreenSize();
  //static float GetScreenSizePercentage();

protected:
	Camera *m_camera;
	CameraComponent m_cameraComponent;
	glm::vec2 m_screenSize;
	glm::vec2 m_defaultScreenSize;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	/*
  static unsigned int m_VBO, m_VAO;
  static Shader s_basicShader;
  static Shader s_uiShader;
  static Shader s_circleShader;
  static Shader s_textureShader;
  static glm::vec2 s_screenSize;
  static glm::vec2 s_screenOffset;
  static glm::vec2 s_defaultScreenSize;
  */
	friend class RenderSystem;
  
};
