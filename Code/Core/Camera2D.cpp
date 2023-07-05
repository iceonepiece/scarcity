#include "Camera2D.h"
#include <glm/gtc/matrix_transform.hpp>

Camera2D::Camera2D(glm::vec3 position, glm::vec2 boxSize, glm::vec2 screenSize)
    : Camera(position, screenSize)
    , m_boxSize(boxSize)
    , m_body(nullptr)
{
}

void Camera2D::Update()
{
  if (m_body)
  {
    b2Vec2 position = m_body->GetPosition();

    float newX = m_position.x * -1;
    if (position.x > newX + m_boxSize.x)
      m_position.x -= position.x - (newX + m_boxSize.x);
    else if (position.x < newX - m_boxSize.x)
      m_position.x += (newX - m_boxSize.x) - position.x;

    float newY = m_position.y * -1;
    if (position.y > newY + m_boxSize.y)
      m_position.y -= position.y - (newY + m_boxSize.y);
    else if (position.y < newY - m_boxSize.y)
      m_position.y += (newY - m_boxSize.y) - position.y;
  }
}

glm::mat4 Camera2D::GetViewMatrix() const
{
    return glm::translate(glm::mat4(1.0f), m_position);
}

glm::mat4 Camera2D::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(45.0f), m_screenSize.x / m_screenSize.y, 0.1f, 100.0f);
}

void Camera2D::SetBody(b2Body* body)
{
  m_body = body;
}
