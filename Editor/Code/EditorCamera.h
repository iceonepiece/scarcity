#pragma once

#include "Core/Camera.h"

class EditorCamera : public Camera
{
public:
	EditorCamera()
		: Camera(glm::vec3(0.0f), glm::vec2(50, 50), glm::vec2(1280, 720))
	{

	}

	glm::mat4 GetProjection()
	{
		return glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);
	}
};