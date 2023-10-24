#pragma once

#include <glm/glm.hpp>

class Math
{
public:
    static bool Contains(const glm::vec2& position, const glm::vec2& size, const glm::vec2& point);
    static glm::vec2 ConvertToNDC(const glm::vec2& screenPos, const glm::vec2& screenSize);
    static glm::vec2 ConvertToWorldSpace(const glm::vec2& ndcPos, const glm::mat4& inverseProjectionMatrix, const glm::mat4& inverseViewMatrix);
};