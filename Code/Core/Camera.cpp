#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec2 boxSize, glm::vec2 screenSize)
    : m_position(position)
    , m_boxSize(boxSize)
    , m_screenSize(screenSize)
    , m_body(nullptr)
{
}

void Camera::Update()
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

glm::vec3 Camera::GetPosition()
{
  return m_position;
}

glm::vec2 Camera::GetScreenSize()
{
  return m_screenSize;
}

void Camera::SetPosition(glm::vec3 position)
{
  m_position = position;
}

void Camera::SetScreenSize(glm::vec2 screenSize)
{
  m_screenSize = screenSize;
}

void Camera::SetBody(b2Body* body)
{
  m_body = body;
}
