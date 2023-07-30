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
    if (m_type == CameraType::Orthographic)
    {
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
        return glm::lookAt(m_position, m_position + front, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    return glm::translate(glm::mat4(1.0f), m_position);
}

glm::mat4 Camera2D::GetProjectionMatrix() const
{
    return GetProjectionMatrix(m_type);
}

glm::mat4 Camera2D::GetProjectionMatrix(CameraType type) const
{
    float ratio = m_screenSize.x / m_screenSize.y;

    if (type == CameraType::Perspective)
        return glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);

    float width = m_orthographicSize * ratio;

    if (type == CameraType::Orthographic)
        return glm::ortho(-width / m_zoom, width / m_zoom, -m_orthographicSize / m_zoom, m_orthographicSize / m_zoom);

    return glm::mat4(1.0f);
}

void Camera2D::SetBody(b2Body* body)
{
  m_body = body;
}
