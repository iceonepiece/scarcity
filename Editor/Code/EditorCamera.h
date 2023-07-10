#pragma once

#include "Core/Camera.h"

class EditorCamera : public Camera
{
public:
	EditorCamera()
		: Camera(glm::vec3(0.0f), glm::vec2(1280, 720))
	{

	}

	virtual void Update() override {

	}

	virtual glm::mat4 GetProjectionMatrix() const override
	{
		return glm::perspective(glm::radians(45.0f), m_screenSize.x / m_screenSize.y, 0.1f, 100.0f);
	}

	virtual glm::mat4 GetProjectionMatrix(CameraType type) const override
	{
		return glm::perspective(glm::radians(45.0f), m_screenSize.x / m_screenSize.y, 0.1f, 100.0f);
	}

	virtual glm::mat4 GetViewMatrix() const override
	{
		return glm::translate(glm::mat4(1.0f), m_position);
	}
};