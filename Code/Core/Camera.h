#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <box2d/box2d.h>

enum class CameraType
{
	Perspective,
	Orthographic
};

class Camera
{
public:
	Camera() = default;
	Camera(const glm::vec3& position, const glm::vec2& screenSize)
		: m_position(position)
		, m_screenSize(screenSize)
		, m_defaultScreenSize(screenSize)
		, m_type(CameraType::Perspective)
	{}

	inline void SetCameraType(CameraType type) { m_type = type; }
	inline CameraType GetCameraType() { return m_type; }

	inline glm::vec3 GetPosition() const { return m_position; }
	inline void SetPosition(const glm::vec3& position) { m_position = position; }

	inline glm::vec2 GetScreenSize() const { return m_screenSize; }
	inline void SetScreenSize(const glm::vec2& screenSize) { m_screenSize = screenSize; }

	virtual void SetBody(b2Body* body) {}

	virtual void Update() = 0;
	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix(CameraType type) const = 0;
	
	inline float GetScreenSizePercentage() { return m_screenSize.x / m_defaultScreenSize.x; }
	
	inline float GetZoom() { return m_zoom; }
	inline void SetZoom(float zoom) { m_zoom = zoom; }

protected:
	float m_zoom = 1.0f;
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec2 m_screenSize;
	glm::vec2 m_defaultScreenSize;
	CameraType m_type;
};

