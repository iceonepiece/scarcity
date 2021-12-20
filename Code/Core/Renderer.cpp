#include "Renderer.h"
#include <iostream>

unsigned int Renderer::m_VBO = 0;
unsigned int Renderer::m_VAO = 0;
Shader Renderer::m_shader;

const char* vertexShaderSource = R"(
  #version 330 core
  layout (location = 0) in vec2 aPos;

  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;

  void main()
  {
    gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
  }
)";

const char* fragmentShaderSource = R"(
  #version 330 core
  out vec4 FragColor;

  void main()
  {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
  }
)";

void Renderer::Init()
{
  float vertices[] = {
    0.5f,  0.5f,
    0.5f, -0.5f,
   -0.5f,  0.5f,
    0.5f, -0.5f,
   -0.5f, -0.5f,
   -0.5f,  0.5f
  };

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(m_VAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  m_shader.Compile(vertexShaderSource, fragmentShaderSource);
}

void Renderer::DrawQuad(b2Body* body)
{
  m_shader.Use();

  b2Vec2 position = body->GetPosition();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));

  b2Fixture* fixture = body->GetFixtureList();
  if (fixture)
  {
    b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
    float width = shape->m_vertices[1].x - shape->m_vertices[0].x;
    float height = shape->m_vertices[2].y - shape->m_vertices[0].y;

    model = glm::scale(model, glm::vec3(width, height, 0.0f));
  }

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -40.0f));

  glm::mat4 projection = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);

  m_shader.SetMatrix4("model", model);
  m_shader.SetMatrix4("view", view);
  m_shader.SetMatrix4("projection", projection);

  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
