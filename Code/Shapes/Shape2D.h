#pragma once

#include <glm/glm.hpp>

struct Shape2D
{
	glm::vec4 color = glm::vec4 { 1.0f };
	glm::vec2 position = glm::vec2{ 0.0f };
	glm::vec2 scale = glm::vec2{ 1.0f };
	float angle = 0.0f;

    static bool IsPointOnRectangle(const glm::vec2& point, const glm::vec2& rectPosition, const glm::vec2& scale, float angle) {
        // Translate the point to the local coordinate system of the rectangle
        glm::vec2 localPoint(point.x - rectPosition.x, point.y - rectPosition.y);

        // Inverse rotate the point
        float cosAngle = std::cos(-angle);
        float sinAngle = std::sin(-angle);
        glm::vec2 rotatedPoint(
            localPoint.x * cosAngle - localPoint.y * sinAngle,
            localPoint.x * sinAngle + localPoint.y * cosAngle
        );

        // Check if the rotated point is inside the unrotated rectangle
        bool IsOnX = std::abs(rotatedPoint.x) <= scale.x / 2.0;
        bool IsOnY = std::abs(rotatedPoint.y) <= scale.y / 2.0;

        return IsOnX && IsOnY;
    }
};

struct Quad2D : public Shape2D
{
};

struct Circle2D : public Shape2D
{
    float radius = 0.5f;
};