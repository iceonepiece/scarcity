#pragma once

#include <glad/glad.h>
#include <box2d/box2d.h>

#include "Shader.h"
#include "../Core/Camera.h"
#include "../UIs/UI.h"
#include "../Core/Texture.h"
#include "../Core/Sprite.h"

class Renderer
{
public:
	virtual void Initialize() = 0;
	virtual void Draw(Sprite& sprite, const glm::mat4& modelMatrix) = 0;
	virtual void DrawLine(const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& color = glm::vec4(1)) = 0;
	virtual void DrawRect(b2Body* body, const Camera& camera) = 0;

	virtual void DrawCircle(const glm::vec2& position, float radius) = 0;

	void SetCamera(Camera* camera);

  static void Init();
  static void DrawQuad(b2Body* body, Camera& camera);
  static void DrawCircle(b2Body* body, glm::vec4 color, Camera& camera);
  static void DrawQuadUI(glm::vec2 position, glm::vec2 scale, glm::vec4 color, UIAlignment alignment = UIAlignment::NONE);
  static void DrawCircle(glm::vec2 position, float radius, glm::vec4 color, Camera& camera);
  static void DrawSprite(Sprite& sprite, glm::vec2 position, glm::vec2 scale, Camera& camera);
  static void SetScreenSize(int width, int height, int xOffset, int yOffset);
  static glm::vec2 GetScreenSize();
  static float GetScreenSizePercentage();

protected:
	Camera *m_camera;

  static unsigned int m_VBO, m_VAO;
  static Shader s_basicShader;
  static Shader s_uiShader;
  static Shader s_circleShader;
  static Shader s_textureShader;
  static glm::vec2 s_screenSize;
  static glm::vec2 s_screenOffset;
  static glm::vec2 s_defaultScreenSize;
  
};
