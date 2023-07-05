#pragma once

#include <glm/glm.hpp>
#include <box2d/box2d.h>

class Camera
{
public:
	Camera() = default;
	Camera(const glm::vec3& position, const glm::vec2& screenSize)
		: m_position(position)
		, m_screenSize(screenSize)
	{}

	inline glm::vec3 GetPosition() const { return m_position; }
	inline void SetPosition(const glm::vec3& position) { m_position = position; }

	inline glm::vec2 GetScreenSize() const { return m_screenSize; }
	inline void SetScreenSize(glm::vec2 screenSize) { m_screenSize = screenSize; }

	virtual void SetBody(b2Body* body) = 0;

	virtual void Update() = 0;
	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const = 0;

protected:
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec2 m_screenSize;
};

