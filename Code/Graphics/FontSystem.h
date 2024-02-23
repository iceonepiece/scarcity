#pragma once

class FontSystem
{
public:
    virtual ~FontSystem() = default;
    virtual int Initialize() = 0;
    virtual void RenderText(const std::string& text, const glm::vec2& position, float scale, const glm::vec4& color, const glm::vec2& viewportSize) = 0;
};