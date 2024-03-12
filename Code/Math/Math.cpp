#include "Math.h"

bool Math::Contains(const glm::vec2& position, const glm::vec2& size, const glm::vec2& point)
{
    float halfWidth = size.x / 2;
    float halfHeight = size.y / 2;

    return !(point.x < position.x - halfWidth
        || point.x > position.x + halfWidth
        || point.y > position.y + halfHeight
        || point.y < position.y - halfHeight
    );
}

glm::vec2 Math::ConvertToNDC(const glm::vec2& screenPos, const glm::vec2& screenSize)
{
    float normalizedX = screenPos.x / screenSize.x;
    float normalizedY = screenPos.y / screenSize.y;

    float ndcX = (normalizedX * 2.0f) - 1.0f;
    float ndcY = (normalizedY * -2.0f) + 1.0f;

    return { ndcX, ndcY };
}

glm::vec2 Math::ConvertToWorldSpace(const glm::vec2& ndcPos, const glm::mat4& inverseProjectionMatrix, const glm::mat4& inverseViewMatrix)
{
    glm::vec4 ndcPosition = glm::vec4(ndcPos.x, ndcPos.y, 0.0f, 1.0f);

    glm::vec4 clipSpacePosition = inverseProjectionMatrix * ndcPosition;
    clipSpacePosition /= clipSpacePosition.w;

    glm::vec4 worldSpacePosition = inverseViewMatrix * clipSpacePosition;

    return { worldSpacePosition.x, worldSpacePosition.y };
}