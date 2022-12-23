#pragma once

#include <glad/glad.h>
#include <box2d/box2d.h>

#include "Shader.h"
#include "../Core/Camera.h"

class Renderer
{
public:
  static void Init();
  static void DrawQuad(b2Body* body, Camera& camera);
  static void DrawQuadUI(glm::vec2 position, glm::vec2 scale, glm::vec4 color);

private:
  static unsigned int m_VBO, m_VAO;
  static Shader s_basicShader;
  static Shader s_uiShader;
};
