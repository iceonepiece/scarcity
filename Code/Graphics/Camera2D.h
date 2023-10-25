#pragma once

#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Camera.h"

class Camera2D : public Camera
{
public:
	Camera2D(glm::vec3 position, glm::vec2 boxSize, glm::vec2 screenSize);

	virtual glm::mat4 GetViewMatrix() const override;
	virtual glm::mat4 GetProjectionMatrix() const override;
	virtual glm::mat4 GetProjectionMatrix(CameraType type) const override;
	virtual glm::vec4 ScreenToWorldPosition(const glm::vec2& screenPosition) const override;
	
	virtual void Update() override;
	virtual void SetBody(b2Body* body) override;

private:
	float m_orthographicSize = 4.0f;
	b2Body* m_body;
	glm::vec2 m_boxSize;
};