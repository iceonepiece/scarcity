#pragma once

#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <iostream>

class Camera
{
public:
  Camera(glm::vec3 position, glm::vec2 boxSize);

  void Update();
  glm::vec3 GetPosition();
  void SetPosition(glm::vec3 position);
  void SetBody(b2Body* body);

private:
  b2Body* m_body;
  glm::vec3 m_position;
  glm::vec2 m_boxSize;
};
