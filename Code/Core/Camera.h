#pragma once

#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec2 boxSize, glm::vec2 screenSize);
	
	void Update();
	glm::vec3 GetPosition();
	glm::vec2 GetScreenSize();
	glm::mat4 GetViewMatrix();
	void SetPosition(glm::vec3 position);
	void SetScreenSize(glm::vec2 screenSize);
	void SetBody(b2Body* body);

private:
	b2Body* m_body;
	glm::vec3 m_position;
	glm::vec2 m_boxSize;
	glm::vec2 m_screenSize;
};
