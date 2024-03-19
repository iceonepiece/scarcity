#pragma once

#include <string>
#include <unordered_map>
#include "Graphics/Font.h"
#include "UI/UIBase.h"

class FontSystem
{
public:
    virtual ~FontSystem() = default;
    virtual int Initialize() = 0;
    virtual void RenderText(const std::string& text, const glm::vec2& position, float scale, const glm::vec4& color, const glm::vec2& viewportSize, UIFlag flag = UIFlag_None) = 0;

    virtual void LoadFont(Font& font) = 0;

    void SetFont(const std::string& name)
    {
        if (m_fontMap.find(name) != m_fontMap.end())
            m_currentFont = m_fontMap[name];
    }

    void SetScreenSize(const glm::vec2& size)
    {
        m_screenSize = size;
    }

protected:
    inline static std::unique_ptr<Font> s_defaultFont = nullptr;

    glm::vec2 m_screenSize { 1.0f };

    std::unordered_map<std::string, Font*> m_fontMap;
    Font* m_currentFont = nullptr;

    friend class Renderer;
};