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

private:
  static unsigned int m_VBO, m_VAO;
  static Shader m_shader;
};
